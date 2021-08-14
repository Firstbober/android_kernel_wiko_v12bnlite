/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/qdsp6v2/apr.h>
#include <linux/of_device.h>
#include <linux/sysfs.h>
#include <soc/qcom/subsystem_restart.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/fs.h>
#include <linux/uaccess.h>

#define MISC_PATH  "/dev/block/bootdevice/by-name/misc"
#define OFFSET_FLAG  4096
#define MAX_MODULE 64	 
#define MAX_BUF 1024
//extern char kernellogmaskbuf[1024];

char maskbuf[MAX_BUF]={0};
static  int use_cnt = 0;
char reset[] = "miscreset";

struct misc_rw_private {
	struct kobject *misc_rw_obj;
	struct attribute_group *attr_group;
};

struct module_para {
	char  module[8];
	char  mask[8];
};
static struct module_para m_module_para[MAX_MODULE];
static ssize_t misc_rw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

char * module_parser_mask(char *module)
{
	int i;
    if(module == NULL)
    {
    	printk("%s, input module name is NULL !!!",__func__);
		return NULL;		
    }
	
	for(i=0; i<use_cnt;i++)
	{
		if(!strncmp(m_module_para[i].module,module,strlen(module)))
			return m_module_para[i].mask;
	}
	return NULL;
}
EXPORT_SYMBOL(module_parser_mask);


int parser_kernellogmaskbuf(char *ptr)
{
	int i;
	char *p = NULL;
	char *p1 = NULL;
	char a[1024]= {0};	
	char *temp = a;
	for(i = 0; i<64; i++)
	{	
		memset(m_module_para[i].module,0,8);
		memset(m_module_para[i].mask,0,8);
	}
	
	if(ptr == NULL)
	{		
		printk("%s,ptr null\n",__func__);
		goto exit;
	}
	
	if(strlen(ptr)== 0)
	{
		printk("%s,length 0\n",__func__);
		goto exit;	
	}
	
	//temp = kstrndup(ptr, MAX_BUF, GFP_KERNEL);
	//if (!temp) {
	//	printk("%s: failed to copy string\n",__func__);
	//	return -ENOMEM;
	//}
	strcpy(temp,(const char *) ptr);
	while(strlen(temp))
	{		
		p = strsep(&temp,";");
		if(p)
		{
			p1 = strsep(&p,"_");
			strcpy(m_module_para[use_cnt].module,p1);
			strcpy(m_module_para[use_cnt].mask,p);
			use_cnt++;
		}
	}
	for(i = 0; i<use_cnt; i++)
	{
		strcat(maskbuf,m_module_para[i].module);strcat(maskbuf,"_");
		strcat(maskbuf,m_module_para[i].mask);strcat(maskbuf,";");
	}
	printk("%s, maskbuf: %s\n",__func__,maskbuf);
	//if(temp != NULL)
	//{
	//	kfree(temp);
	//	temp = NULL;
	//}
exit:
	return 0;	
}
EXPORT_SYMBOL(parser_kernellogmaskbuf);

static ssize_t misc_rw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	printk("%s, maskbuf: %s\n",__func__,maskbuf);

	return snprintf(buf, strlen(maskbuf)+8, "mask:%s\n",maskbuf);
}

static struct kobj_attribute misc_rw_attribute =
	__ATTR(para, 0660, misc_rw_show, misc_rw_store);

static struct attribute *attrs[] = {
	&misc_rw_attribute.attr,	 
	NULL,
};

static void write_misc(char *para)
{
	mm_segment_t fs;   
	struct file *ps_filp = NULL;
	if(para == NULL)
	{
		printk("%s, para is NULL!!\n",__func__);
		return;
	}
		
	fs=get_fs();   

	set_fs(KERNEL_DS);  
	    
	ps_filp = filp_open(MISC_PATH, O_WRONLY , 0);

	if (IS_ERR(ps_filp))
	{
	    pr_info("%s    failed to open  %s\n", __func__, MISC_PATH);
	}
	else
	{
	    ps_filp->f_op->llseek(ps_filp, OFFSET_FLAG, SEEK_SET);
	    ps_filp->f_op->write(ps_filp, para, MAX_BUF, &ps_filp->f_pos);			
	}

	if (ps_filp && !IS_ERR(ps_filp))
	{
	    filp_close(ps_filp, NULL);
	}

	set_fs(fs);  
}
 
static ssize_t misc_rw_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
	char *p = NULL;
	//char *temp = NULL;
	int i,n = 0;
	int flag = 0;//1 => new item;2 => old item new value 
	char a[17]= {0};	
	char *temp = a;
	struct module_para module_para_t;
	
	if(!strncmp(buf,reset,strlen(reset)))
	{ 
		memset(maskbuf,0,1024); 
		write_misc(maskbuf);			
		return count;
	}

	if(use_cnt > MAX_MODULE)
	{     
		printk("%s, use_cunt = %d\n",__func__,use_cnt);
		return -ENOMEM;
	}

	//temp = kstrndup(buf, 17, GFP_KERNEL);
	//if (!temp) {
	//	printk("%s: failed to copy string\n",__func__);
	//	return -ENOMEM;
	//}
	strncpy(temp,buf,16);
	for(i = 0; i< 16;i++)
	{
		if((temp[i] == '_')&&(temp[0] != '_')&&(temp[strlen(temp)-1] != '_'))
		n++;
	}

	if(n == 1)
	{
		p = strsep(&temp,"_");
		if(p)
		{
			strcpy(module_para_t.module,p);

		//p = strsep(&temp,"_");		
		//if(p)		
			strncpy(module_para_t.mask,temp,7);
		}
		for(i = 0; i<use_cnt; i++)
		{
			if(!strcmp(module_para_t.module,m_module_para[i].module))
			{	//old item 			
				if(strcmp(module_para_t.mask,m_module_para[i].mask))
				{//old item  new value				
					flag = 2;
					memset(m_module_para[i].mask,0,8);
					strcpy(m_module_para[i].mask,module_para_t.mask);					
				}		
				break;
			}
		}
		if(i == use_cnt)
			flag = 1;
		//new item
		if(flag == 1)
		{
			strcpy(m_module_para[use_cnt].module,module_para_t.module);
			strcpy(m_module_para[use_cnt].mask,module_para_t.mask);
			use_cnt++;
		}
        if((flag == 2)||(flag == 1))
        {        
			memset(maskbuf,0,1024);
			for(i = 0; i<use_cnt; i++)
			{
				strcat(maskbuf,m_module_para[i].module);strcat(maskbuf,"_");
				strcat(maskbuf,m_module_para[i].mask);strcat(maskbuf,";");
			}
			
			printk("%s,flag = %d, write msic partion(%s)!!\n",__func__,flag,maskbuf);
			write_misc(maskbuf);
        }
	}
    else
    {
    	printk("%s, write parameters(%s) error !!!\n",__func__,temp);
    }
	
	//kfree(temp);
	return count;
}
 
static int misc_rw_init_sysfs(struct platform_device *pdev)
{
	int ret = -EINVAL;
	struct misc_rw_private *priv = NULL;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "%s: memory alloc failed\n", __func__);
		ret = -ENOMEM;
		return ret;
	}

	platform_set_drvdata(pdev, priv);

	priv->misc_rw_obj = NULL;
	priv->attr_group = devm_kzalloc(&pdev->dev,
	 sizeof(*(priv->attr_group)),
	 GFP_KERNEL);
	if (!priv->attr_group) {
		dev_err(&pdev->dev, "%s: malloc attr_group failed\n",
				 __func__);
		ret = -ENOMEM;
		goto error_return;
	}

	priv->attr_group->attrs = attrs;

	priv->misc_rw_obj = kobject_create_and_add("misc-drv", kernel_kobj);
	if (!priv->misc_rw_obj) {
		dev_err(&pdev->dev, "%s: sysfs create and add failed\n",
				 __func__);
		ret = -ENOMEM;
		goto error_return;
	}

	ret = sysfs_create_group(priv->misc_rw_obj, priv->attr_group);
	if (ret) {
		dev_err(&pdev->dev, "%s: sysfs create group failed %d\n",
					 __func__, ret);
		goto error_return;
	}
	
	//parser_kernellogmaskbuf(kernellogmaskbuf);

	return 0;

error_return:

	if (priv->misc_rw_obj) {
		kobject_del(priv->misc_rw_obj);
		priv->misc_rw_obj = NULL;
	}
	return ret;
}

static int misc_rw_remove(struct platform_device *pdev)
{
	struct misc_rw_private *priv = NULL;

	priv = platform_get_drvdata(pdev);

	if (!priv)
		return 0;

	if (priv->misc_rw_obj) 
	{
		sysfs_remove_group(priv->misc_rw_obj, priv->attr_group);
		kobject_del(priv->misc_rw_obj);
		priv->misc_rw_obj = NULL;
	}
	return 0;
}

static int misc_rw_probe(struct platform_device *pdev)
{
	int ret = misc_rw_init_sysfs(pdev);
	if (ret != 0) 
	{
		dev_err(&pdev->dev, "%s: Error in initing sysfs\n", __func__);
		return ret;
	}
	return 0;
}
 
static const struct of_device_id misc_rw_dt_match[] = {
	{ .compatible = "qcom,log-ctrl" },
	{ }
};
MODULE_DEVICE_TABLE(of, misc_rw_dt_match);

static struct platform_driver misc_driver = {
	.driver = {
	.name = "LOG-CTRL",
	.owner = THIS_MODULE,
	.of_match_table = misc_rw_dt_match,
	},
	.probe = misc_rw_probe,
	.remove = misc_rw_remove,
};

static int __init misc_rw_init(void)
{
	return platform_driver_register(&misc_driver);
}
module_init(misc_rw_init);

static void __exit misc_rw_exit(void)
{
	platform_driver_unregister(&misc_driver);
}
module_exit(misc_rw_exit);

MODULE_DESCRIPTION("MISC DRV module");
MODULE_LICENSE("GPL v2");

