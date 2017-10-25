
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>        //open
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/sem.h> 
#include <sys/ipc.h>	  //shmget
#include <sys/shm.h>	  //shmget
#include <arpa/inet.h>	  //inet_addr(); inet_ntoa()
#include <sys/socket.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/msg.h>
#include<sys/ioctl.h>
#include <time.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "cgic.h"
#include "web_mgr.h"
#include "mgr.h"

/**
 * js、css文件版本号规划    0~100：v1.0     301~600：v1.1  1001~1500：v1.1
 */

/**
 * @var semid_ipc
 * @brief 进程间通信共享内存信号量IPC键值
 */
int semid_ipc;	 
/**
 * @var shm_ipc_addr
 * @brief 进程间通信共享内存地址
 */
int shmid_ipc;	
SHM_IPC_STRU * shm_ipc_addr = NULL;
/**
 * @var shm_cfg_addr
 * @brief 配置区共享内存地址
 */
SHM_CFG_STRU * shm_cfg_addr = NULL;
/**
 * @var shmid_cfg
 * @brief 配置区共享内存IPC键值
 */
int shmid_cfg;	 
/**
 * @var semid_cfg
 * @brief 配置区共享内存信号量IPC键值
 */
int semid_cfg;	 
/**
 * @var  qid_mgrh
 * @brief 消息队列键值
 */
int qid_mgrh;
/**
 * @var web_data
 * @brief 存储共享内存/配置文件数据
 */
WEB_NM_DATA_ITEM     web_data;
/**
 * @var eeprom_esn_param
 * @brief 读取MAC地址
 */
EEPROM_ESN_PARAM eeprom_esn_param;
int eeprom_fd;
unsigned char buf_read[2];


/**
 * @var md5_sum_in_bin
 * @brief 从bin文件中提取的md5校验码
 */
char md5_sum_in_bin[MD5_LEN + 1] = {0};

/**
 * @var md5_sum
 * @brief 计算的md5校验码
 */
char md5_sum[MD5_LEN + 1] = {0};
unsigned int A,B,C,D,a,b,c,d,flen[2],x[16];
int len;
char filename[200];
FILE *fp;
char buffer[30]= {0};///存放挂载点

/**
 * @show_login    登录页面
 *
 * @param[in]		web_user		共享内存保存的用户名
 * @param[in]		web_code		共享内存保存的密码
 * @param[in]		flag			登录状态 1：用户模式  2：工程模式  3：登录信息错误
 * @param[in]		language_switch   语言开关   0：中文  1：英文
 * @return		void
 * @author		wdz
 * @bug
 */
void show_login( char * web_user, char * web_code,int flag,int language_switch)
{
	fprintf(cgiOut, "<!DOCTYPE html>\n");
	fprintf(cgiOut, "<html>\n");
	fprintf(cgiOut, "<head>\n");
	
	fprintf(cgiOut, "<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"utf-8\" />\n"); 
	fprintf(cgiOut, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n");
	fprintf(cgiOut, "<meta http-equiv=\"Pragma\" content=\"no-cache\"> \n");
	fprintf(cgiOut, "<meta http-equiv=\"Cache-Control\" content=\"no-cache\"> \n");
	fprintf(cgiOut, "<meta http-equiv=\"Expires\" content=\"0\">\n");
	fprintf(cgiOut, "<meta http-equiv=\"Cache\" content=\"no-cache\">\n");
	
	fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../css/style.css?v=303\">\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery_1_7_2_min.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/layer.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/EpackConf.js?v=303\"></script>\n");
	fprintf(cgiOut,"<script>\n");
	fprintf(cgiOut,"$(document).ready(function(){\n");
	fprintf(cgiOut,"		disable_back();\n");
	fprintf(cgiOut,"				log();\n");
	fprintf(cgiOut,"				log_type(\"%d\",\"%s\",\"%s\",\"%d\");\n",flag,web_user,web_code,language_switch);
	fprintf(cgiOut,"				})\n"); 
	fprintf(cgiOut,"</script>\n");
	
	fprintf(cgiOut, "</head>\n");

	fprintf(cgiOut, "<body  id=\"login\" oncontextmenu = \"return false\" onkeydown=\"Enter(event)\">\n");
	fprintf(cgiOut, "	<div class=\"page-container\">\n");
	fprintf(cgiOut, "		<h1 id=\"h1_text\">E-pack100 单机网络管理系统</h1>\n");
	fprintf(cgiOut, "		<form id=\"form2\" action=\"web_mgr.cgi\" method=\"post\">\n");
	fprintf(cgiOut, "			<div>\n");
	fprintf(cgiOut, "				<input type=\"text\" id=\"用户名\" name=\"user\" class=\"username\" placeholder=\"User Name\" maxlength=\"20\" autocomplete=\"off\"/>\n");
	fprintf(cgiOut, "			</div>\n");
	
	fprintf(cgiOut, "			<div>\n");
	fprintf(cgiOut, "			<input type=\"password\" id=\"密码\" name=\"code\" class=\"password\" placeholder=\"Password\" maxlength=\"20\" oncontextmenu=\"return false\" onpaste=\"return false\" />\n");
	fprintf(cgiOut, "			</div>\n");
	
	fprintf(cgiOut, "			<div>\n");
	fprintf(cgiOut, "			<select class=\"language\" id=\"language_switch\"  name=\"language_switch\" onchange=\"chang_text()\" >\n");
	fprintf(cgiOut, "				<option value='0' selected>Chinese-中文</option>\n");
	fprintf(cgiOut, "				<option value='1'>English</option>\n");
	fprintf(cgiOut, "			<select>\n");
	fprintf(cgiOut, "			</div>\n");
	
	fprintf(cgiOut, "			<button id=\"log\" type=\"button\" >登　　录</button>\n");
	fprintf(cgiOut, "			<input name=\"submit\" id=\"submit\"  type=\"submit\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "			<input name=\"normal_mode\" id=\"normal_mode\"  type=\"submit\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "			<input name=\"engineer_pattern\" id=\"engineer_pattern\"  type=\"submit\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "			<input id=\"web_code\" type=\"text\" name=\"web_code\"  value =\"%s\" class=\"ipt-text \"  style=\"display:none\" >\n",web_code);
	fprintf(cgiOut, "			<input id=\"web_user\" type=\"text\" name=\"web_user\"  value =\"%s\" class=\"ipt-text \"  style=\"display:none\" >\n",web_user);
	fprintf(cgiOut, "		</form >\n");
	
	fprintf(cgiOut, "		<div class=\"connect\">\n");
	fprintf(cgiOut, "			<p id=\"p_text\">版权所有 © 1993-2017 海能达通信股份有限公司</p>\n");
	fprintf(cgiOut, "		</div>\n");
	fprintf(cgiOut, "	</div>\n");
	fprintf(cgiOut, "</body>\n");
	fprintf(cgiOut, "</html>\n");
}


/**
 * @showHtml    配置页面
 *
 * @param[in]		engineer_mode	当前模式：用户/工程   0：用户  1：工程
 * @param[in]		flag		    操作返回状态标志
 * @param[in]		web_data_point  配置项数据
 * @param[in]		current_div     当前显示div
 * @param[in]		language_switch     语言开关
 * @param[in]		file_name       升级文件名
 * @return		void
 * @author		wdz-6
 * @bug
 */
 
void showHtml(int engineer_mode ,int flag,WEB_NM_DATA_ITEM * web_data_point,int current_div, char *file_name,int language_switch)
{
	fprintf(cgiOut, "<!DOCTYPE html>\n");
	fprintf(cgiOut, "<html>\n");
	fprintf(cgiOut, "<head>\n");
	
	fprintf(cgiOut, "<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"utf-8\" />\n"); 
	fprintf(cgiOut, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n");
	fprintf(cgiOut, "<meta http-equiv=\"Pragma\" content=\"no-cache\"> \n");
	fprintf(cgiOut, "<meta http-equiv=\"Cache-Control\" content=\"no-cache\"> \n");
	fprintf(cgiOut, "<meta http-equiv=\"Expires\" content=\"0\">\n");
	fprintf(cgiOut, "<meta http-equiv=\"Cache\" content=\"no-cache\">\n");
	
	fprintf(cgiOut, "<link rel=\"stylesheet\" type=\"text/css\" href=\"../css/jquery-ui.css?v=303\" />\n");
	fprintf(cgiOut, "<link rel=\"stylesheet\" type=\"text/css\" href=\"../css/validationEngine.jquery.css?v=303\" />\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery_1_7_2_min.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery.form.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/EpackConf.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery-ui.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/FileSaver.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/FileSaver.min.js?v=303\"></script>\n");
	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/layer.js?v=303\"></script>\n");
//	fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/extend/layer.ext.js?v=8\"></script>\n");
	
    fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery.validationEngine.min.js?v=303\"></script>\n");
	
    fprintf(cgiOut, "<meta charset=\"utf-8\">\n");	
	fprintf(cgiOut, "<title id=\"title_text\">E-pack100 单机网络管理系统 - 配置</title>\n");
    fprintf(cgiOut, "<meta name=\"viewport\" content=\"width=device-width\" initial-scale=\"1.0\">\n");
    fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../iconfont/iconfont.css?v=303\">\n");
	if(language_switch==0)
	{
		fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery.validationEngine-zh_CN.js?v=303\"></script>\n");
		fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../css/style.css?v=303\">\n");
	}
	else
	{
		fprintf(cgiOut, "<script type=\"text/javascript\" src=\"../js/jquery.validationEngine-en.js?v=303\"></script>\n");
		fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../css/style_en.css?v=303\">\n");
	}
	
	
	////////////////////////////
	
	fprintf(cgiOut,"<script>\n");
	
	fprintf(cgiOut,"$(document).ready(function(){\n");
	fprintf(cgiOut,"		disable_back();\n");
	fprintf(cgiOut,"		show_lable();\n");
	fprintf(cgiOut,"		show_title();\n");
//	fprintf(cgiOut,"		disable_flash();\n");
//	fprintf(cgiOut,"		document.onkeydown=disable_shortcut_key();\n"); 565
	fprintf(cgiOut,"		$(function () { \n");
	fprintf(cgiOut,"			$( document ).tooltip({\n");  
    fprintf(cgiOut,"									position: {\n");  
	fprintf(cgiOut,"									my: \"center top\", \n"); 
	fprintf(cgiOut,"									at: \"right bottom+10\" \n"); 
    fprintf(cgiOut,"				}\n"); 
	fprintf(cgiOut,"				})\n");
	fprintf(cgiOut,"				});\n");
	fprintf(cgiOut,"	})\n");
	fprintf(cgiOut,"</script>\n");
	
	fprintf(cgiOut,"<script>\n");
	fprintf(cgiOut,"$(document).ready(function(){\n");
	fprintf(cgiOut,"	$(\"#form1\").validationEngine(\"attach\",{\n");
	fprintf(cgiOut,"						scroll : false,\n");
	fprintf(cgiOut,"						showArrow : false,\n");
	fprintf(cgiOut,"						showPrompts : true,\n");
	fprintf(cgiOut,"						promptPosition : \"inline\",\n");
	fprintf(cgiOut,"						maxErrorsPerField : 1,\n");
	fprintf(cgiOut,"						showOneMessage : true,\n");
    fprintf(cgiOut,"						addPromptClass : \" formError-text\",\n");
    fprintf(cgiOut,"						autoHidePrompt : true,\n");
    fprintf(cgiOut,"						autoHideDelay : 2000,\n");
    fprintf(cgiOut,"						autoPositionUpdate: true,\n");	
	fprintf(cgiOut,"						validationEventTrigger: \"blur\",\n");
	fprintf(cgiOut,"						onFieldSuccess: function(field){ $(field).css(\"border-color\",\"#aaa\");},\n");
	fprintf(cgiOut,"						onFieldFailure: function(field){ $(field).css(\"border-color\",\"red\");},\n");
    fprintf(cgiOut,"				})\n");  
	fprintf(cgiOut,"	showTeam();\n");
	fprintf(cgiOut,"	showFreq();\n");
	if(engineer_mode==ENGINEER_MODE)  ////工程模式
	{	
		fprintf(cgiOut,"	showBlklist();\n");
		fprintf(cgiOut,"	showWhtList();\n");
	}
	fprintf(cgiOut,"	showgsmComm();\n");
	fprintf(cgiOut,"	showOption(\"%u\",\"%u\",\"%u\",\"%u\",\"%u\", \"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\",\"%u\");\n",
	                               web_data_point->dev_power,web_data_point->locking_time,web_data_point->half_variance_threshold,web_data_point->mic_volume,web_data_point->gsm_volume, web_data_point->horn_volume,web_data_point->blacklist_switch,web_data_point->whitelist_switch,web_data_point->filter_switch
								   ,web_data_point->alarm_ai_switch,web_data_point->neighbor_switch,web_data_point->alarm_show_switch,web_data_point->gsm_alarm_show_switch,web_data_point->radio_alarm_show_switch,web_data_point->neighbor_ai_switch,atoi(web_data_point->current_group_id),web_data_point->current_freq,
								   web_data_point->vol_start_threshold,web_data_point->vol_packet_time,web_data_point->terminal_compatible,web_data_point->work_mode_2,web_data_point->vol_code_2,web_data_point->neighbor_period,web_data_point->stop_transmit,web_data_point->device_language_switch,web_data_point->boot_mode_switch,web_data_point->reboot_strategy_switch);
	
	fprintf(cgiOut,"	showDiv(\"%d\");\n",current_div);
	fprintf(cgiOut,"	showTip(\"%d\");\n",flag);
    fprintf(cgiOut,"	})\n");
	fprintf(cgiOut,"</script>\n");
	////////////////////////////////////
	
	////////////////////////////
	

	fprintf(cgiOut, "</head>\n");
	
	
	fprintf(cgiOut, "<body id=\"loading\" oncontextmenu = \"return false\" onkeydown=\"Enter(event)\">\n");
	fprintf(cgiOut, "<form id = \"form1\" name =\"form1\"  method = \"post\" action=\"web_mgr.cgi\" enctype=\"multipart/form-data\" class=\"form-horizontal\">\n");
	
	fprintf(cgiOut, "<div class=\"head\">\n");
	fprintf(cgiOut, "	<div class=\"logo\"><img src=\"../img/logo.png\" height=\"48\"  alt=\"\"/></div>\n");
	fprintf(cgiOut, "	<h1 id=\"h1_text\">E-pack100 单机网络管理系统 </h1>\n");
	fprintf(cgiOut, "	<div class=\"fr\">\n");
	fprintf(cgiOut, "		<a id=\"a1_text\" href=\"#\" onmousedown=\"impot()\"><i class=\"icon iconfont icon-epack-9\"></i>导入</a>\n");
	fprintf(cgiOut, "		<a id=\"a2_text\" href=\"#\" onclick=\"expt()\"><i class=\"icon iconfont icon-epack-10\"></i>导出</a>\n");
	fprintf(cgiOut, "		<a id=\"a3_text\" href=\"#\" onclick=\"submit()\"><i class=\"icon iconfont icon-epack-18\"></i>写入 </a>\n");
	fprintf(cgiOut, "		<a id=\"a4_text\" href=\"#\" onmousedown=\"readParam_validate()\"><i class=\"icon iconfont icon-epack-16\"></i>读取</a>\n");
//	fprintf(cgiOut, "		<a href=\"#\" onclick=\"modify_code()\"><i class=\"icon iconfont icon-epack-19\"></i>修改密码</a> \n");
	fprintf(cgiOut, "		<a id=\"a5_text\" href=\"#\" onclick=\"reboot_validate()\"><i class=\"icon iconfont icon-epack-17\"></i>重启</a> \n");
	
    fprintf(cgiOut, "	</div>\n");
	fprintf(cgiOut, "</div>\n");
	
	fprintf(cgiOut, "<div class=\"left\">\n");
	fprintf(cgiOut, "	<ul id=\"ulList\" class=\"menu\">\n");
	fprintf(cgiOut, "		<li id=\"li1\"  class=\"on\"><a id=\"a6_text\" href=\"javascript:void(0)\" onclick=\"showDiv(1)\"><i class=\"icon iconfont icon-epack-1\"></i>网络参数</a></li>\n");
	fprintf(cgiOut, "		<li id=\"li2\"><a id=\"a7_text\" href=\"javascript:void(0)\" onclick=\"showDiv(2)\"><i class=\"icon iconfont icon-epack-2\"></i>基础设置</a></li>\n");
	fprintf(cgiOut, "		<li id=\"li3\"><a id=\"a8_text\" href=\"javascript:void(0)\" onclick=\"showDiv(3)\"><i class=\"icon iconfont icon-epack-3\"></i>组呼设置</a></li>\n");
	fprintf(cgiOut, "		<li id=\"li4\"><a id=\"a9_text\" href=\"#\" onclick=\"showDiv(4)\"><i class=\"icon iconfont icon-epack-4\"></i>频率设置</a></li>\n");
	fprintf(cgiOut, "		<li id=\"li5\"><a id=\"a10_text\" href=\"#\" onclick=\"showDiv(5)\"><i class=\"icon iconfont icon-epack-5\"></i>GSM通讯录</a></li>\n");
	fprintf(cgiOut, "		<li id=\"li6\"><a id=\"a11_text\" href=\"#\" onclick=\"showDiv(6)\"><i class=\"icon iconfont icon-epack-12\"></i>&nbsp;升&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;级 </a></li>\n");
	
	if(engineer_mode==ENGINEER_MODE)  ////工程模式
	{
		fprintf(cgiOut, "		<li id=\"li8\" ><a id=\"a12_text\" href=\"#\" onclick=\"showDiv(8)\"><i class=\"icon iconfont icon-epack-15\"></i>黑名单设置</a></li>\n");
		fprintf(cgiOut, "		<li id=\"li9\" ><a id=\"a13_text\" href=\"#\" onclick=\"showDiv(9)\"><i class=\"icon iconfont icon-epack-14\"></i>白名单设置</a></li>\n");
		fprintf(cgiOut, "		<li id=\"li10\"><a id=\"a14_text\" href=\"#\" onclick=\"showDiv(10)\"><i class=\"icon iconfont icon-epack-13\"></i>中转台设置</a></li>\n");
		fprintf(cgiOut, "		<li id=\"li11\"><a id=\"a15_text\" href=\"#\" onclick=\"showDiv(11)\"><i class=\"icon iconfont icon-epack-20\"></i>工程模式</a></li>\n");
	}
	else
	{
		fprintf(cgiOut, "		<li id=\"li8\"><a id=\"a16_text\" href=\"#\"  onclick=\"showDiv(8)\"><i class=\"icon iconfont icon-epack-21\"></i>用户管理</a></li>\n");
	}
	fprintf(cgiOut, "		<li id=\"li7\"><a id=\"a17_text\" href=\"#\" onclick=\"showDiv(7)\"><i class=\"icon iconfont icon-epack-6\"></i>版本信息</a></li>\n");
	fprintf(cgiOut, "	</ul>\n");
	fprintf(cgiOut, "	</div>\n");
	
	
	fprintf(cgiOut, "<div class=\"right\">\n");
	fprintf(cgiOut, "	<div id=\"myupload\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input id=\"uploadfile\" name=\"uploadfile\" type=\"file\" onchange=\"createUploadFile()\" style=\"display:none\"/>\n");
	fprintf(cgiOut, "		<input id=\"current_div\" name=\"current_div\" type=\"text\" style=\"display:none\" />\n");
	fprintf(cgiOut, "		<input id=\"current_mode\" name=\"current_mode\" type=\"text\" value =\"%u\" style=\"display:none\" />\n",engineer_mode);
	fprintf(cgiOut, "		<input id=\"language_switch\" name=\"language_switch\" type=\"text\" value =\"%u\" style=\"display:none\" />\n",language_switch);
	fprintf(cgiOut, "		<input id=\"tmp_team\" type=\"text\" name=\"tmp_team\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"tmp_freq\" type=\"text\" name=\"tmp_freq\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"freq_channel\" type=\"text\" name=\"freq_channel\" value =\"%u\" class=\"ipt-text \"  style=\"display:none\" >\n",web_data_point->freq_channel);
	fprintf(cgiOut, "		<input id=\"invalidate_groupNumber\" type=\"text\" name=\"invalidate_groupNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"repeate_groupNumber\" type=\"text\" name=\"repeate_groupNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"invalidate_freqNumber\" type=\"text\" name=\"invalidate_freqNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"repeate_freqNumber\" type=\"text\" name=\"repeate_freqNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"invalidate_blackNumber\" type=\"text\" name=\"invalidate_blackNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"repeate_blackNumber\" type=\"text\" name=\"repeate_blackNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"invalidate_whiteNumber\" type=\"text\" name=\"invalidate_whiteNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"repeate_whiteNumber\" type=\"text\" name=\"repeate_whiteNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"invalidate_gsmNumber\" type=\"text\" name=\"invalidate_gsmNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"repeate_gsmNumber\" type=\"text\" name=\"repeate_gsmNumber\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"read_down\" type=\"text\" name=\"read_down\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input id=\"import_down\" type=\"text\" name=\"import_down\"  class=\"ipt-text \"  style=\"display:none\" >\n");
	if(engineer_mode==NORMAL_MODE)
	{
		fprintf(cgiOut, "		<input id=\"web_code\" type=\"text\" name=\"web_code\"  value =\"%s\" class=\"ipt-text \"  style=\"display:none\" >\n",web_data_point->web_code);
		fprintf(cgiOut, "		<input id=\"web_user\" type=\"text\" name=\"web_user\"  value =\"%s\" class=\"ipt-text \"  style=\"display:none\" >\n",web_data_point->web_user);
	}
    
	fprintf(cgiOut, "		<input name=\"set_code\" id=\"set_code\" type=\"submit\" onclick=\"Delete_validate_Form();\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"set_operate\" id=\"set_operate\"  type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"set_operate_validate\" id=\"set_operate_validate\" type=\"submit\" onclick=\"Delete_validate_Form();\"   style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"get_operate\" id=\"get_operate\" type=\"submit\" onclick=\"Delete_validate_Form();\"   style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"get_operate_validate\" id=\"get_operate_validate\" type=\"submit\" onclick=\"Delete_validate_Form();\"   style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"reboot_operate\" id=\"reboot_operate\" type=\"submit\" onclick=\"Delete_validate_Form();\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"reboot_operate_validate\" id=\"reboot_operate_validate\" type=\"submit\" onclick=\"Delete_validate_Form();\"  style=\"display:none\" >\n");
	fprintf(cgiOut, "		<input name=\"set_code_validate\" id=\"set_code_validate\" type=\"submit\" onclick=\"Delete_validate_Form();\"  style=\"display:none\" >\n");
    fprintf(cgiOut, "		<input name=\"transmit_dtb\" id=\"transmit_dtb\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input name=\"transmit_uboot\" id=\"transmit_uboot\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
    fprintf(cgiOut, "		<input name=\"transmit_loadapp\" id=\"transmit_loadapp\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input name=\"transmit_file\" id=\"transmit_file\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input name=\"transmit_rbf\" id=\"transmit_rbf\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input name=\"transmit_zimage\" id=\"transmit_zimage\" type=\"submit\" onclick=\"Delete_validate_Form();\" style=\"display:none\">\n");
	fprintf(cgiOut, "		<input name=\"again_login\" id=\"again_login\" type=\"submit\" onclick=\"return  false;\" style=\"display:none\">\n");
	fprintf(cgiOut, "	</div>\n");
	
	fprintf(cgiOut, "	<div id=\"show1\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
	fprintf(cgiOut, "			<a name=\"wlcs\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"wlcs_text\">网络参数</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"linkIP_text\" class=\"i-title\">本机IP</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<input id=\"linkIP\" type=\"text\" name=\"linkIP\"     value=\"%s\"  class=\"ipt-text validate[required,custom[ipv4]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\" autocomplete=\"off\" >\n",web_data_point->dev_ip);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"mask_text\" class=\"i-title\">子网掩码</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<input id=\"mask\" type=\"text\" name=\"mask\" value=\"%s\"    class=\"ipt-text validate[required,custom[mask]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->subnet_mask);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gateWay_text\" class=\"i-title\">网关</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<input id=\"gateWay\" type=\"text\" name=\"gateWay\" value=\"%s\"   class=\"ipt-text validate[required,custom[ipv4]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->def_route);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"centerIP_text\" class=\"i-title\">中心IP</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<input id=\"centerIP\" type=\"text\" name=\"centerIP\"  value=\"%s\"  class=\"ipt-text validate[required,custom[ipv4]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->center_ip);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"centerID_text\" class=\"i-title\">互联ID</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<input id=\"centerID\" type=\"text\" name=\"centerID\" value=\"%u\"   class=\"ipt-text validate[required,custom[integer],min[1],max[51200]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->center_id);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	fprintf(cgiOut, "	<div id=\"show2\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
	fprintf(cgiOut, "			<a name=\"jcsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"jcsz_text\">基础设置</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"epkID_text\" class=\"i-title\">本机编号</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"epkID\" type=\"text\" name=\"epkID\" value=\"%u\"    class=\"ipt-text validate[required,custom[integer],min[1],max[32]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->dev_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"intercomID_text\" class=\"i-title\">对讲ID</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"intercomID\" type=\"text\" name=\"intercomID\"  value=\"%s\" class=\"ipt-text validate[required,custom[integer],min[1],max[16776415]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->dev_ai_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"power_text\" class=\"i-title\">发射功率</label>\n");
	fprintf(cgiOut, "					<div id=\"power_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"power\" name=\"power\" >\n");
	fprintf(cgiOut, "							<option id=\"power_1\" value=\"5\">低</option>\n");
	fprintf(cgiOut, "							<option id=\"power_2\" value=\"10\">中</option>\n");
	fprintf(cgiOut, "							<option id=\"power_3\" value=\"20\">高</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"epkCode_text\" class=\"i-title\">链路色码</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"epkCode\" type=\"text\" name=\"epkCode\" value=\"%u\"    class=\"ipt-text validate[required,custom[integer],min[0],max[15]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->dev_cc);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"termCode_text\" class=\"i-title\">终端色码</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"termCode\" type=\"text\" name=\"termCode\" value=\"%u\"  class=\"ipt-text validate[required,custom[integer],min[0],max[15]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->terminal_cc);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"workMode_text\" class=\"i-title\">工作模式</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input  type=\"text\" id=\"workMode\" value=\"%s\" name=\"workMode\"  class=\"ipt-text validate[maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  readonly=\"readonly\" style=\"background-color:#d0d0d0\">\n",web_data_point->work_mode);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"voiceCode_text\" class=\"i-title\">语音编码格式</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input  type=\"text\" id=\"voiceCode\" name=\"voiceCode\"  value=\"%s\"    class=\"ipt-text validate[maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  readonly=\"readonly\" style=\"background-color:#d0d0d0\">\n",web_data_point->vol_code);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"muteThreshold_text\" class=\"i-title\">静噪门限</label>\n");
	fprintf(cgiOut, "					<div id=\"muteThreshold_div\" class=\"select\"  >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"muteThreshold\" name=\"muteThreshold\" >\n");
	fprintf(cgiOut, "							<option id=\"muteThreshold_1\" value=\"3000\">低</option>\n");
	fprintf(cgiOut, "							<option id=\"muteThreshold_2\" value=\"1500\">中</option>\n");
	fprintf(cgiOut, "							<option id=\"muteThreshold_3\" value=\"700\">高</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"micVolume_text\" class=\"i-title\">MIC音量</label>\n");
	fprintf(cgiOut, "					<div id=\"micVolume_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"micVolume\" name=\"micVolume\" >\n");
	fprintf(cgiOut, "							<option id=\"micVolume_1\" value=\"1\">一级</option>\n");
	fprintf(cgiOut, "							<option id=\"micVolume_2\" value=\"2\">二级</option>\n");
	fprintf(cgiOut, "							<option id=\"micVolume_3\" value=\"3\">三级</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmVolume_text\" class=\"i-title\">GSM音量</label>\n");
	fprintf(cgiOut, "					<div id=\"gsmVolume_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"gsmVolume\" name=\"gsmVolume\"  >\n");
	fprintf(cgiOut, "							<option id=\"gsmVolume_1\" value=\"1\">一级</option>\n");
	fprintf(cgiOut, "							<option id=\"gsmVolume_2\" value=\"2\">二级</option>\n");
	fprintf(cgiOut, "							<option id=\"gsmVolume_3\" value=\"3\">三级</option>\n");
	fprintf(cgiOut, "							<option id=\"gsmVolume_4\" value=\"4\">四级</option>\n");
	fprintf(cgiOut, "							<option id=\"gsmVolume_5\" value=\"5\">五级</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"trumpetVolume_text\" class=\"i-title\">手咪默认音量</label>\n");
	fprintf(cgiOut, "					<div id=\"trumpetVolume_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"trumpetVolume\" name=\"trumpetVolume\" >\n");
	fprintf(cgiOut, "							<option id=\"trumpetVolume_1\" value=\"1\">一级</option>\n");
	fprintf(cgiOut, "							<option id=\"trumpetVolume_2\" value=\"2\">二级</option>\n");
	fprintf(cgiOut, "							<option id=\"trumpetVolume_3\" value=\"3\">三级</option>\n");
	fprintf(cgiOut, "							<option id=\"trumpetVolume_4\" value=\"4\">四级</option>\n");
	fprintf(cgiOut, "							<option id=\"trumpetVolume_5\" value=\"5\">五级</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"devCallTimeout_text\" class=\"i-title\">通话限时</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"devCallTimeout\" type=\"text\" name=\"devCallTimeout\" value=\"%u\" class=\"ipt-text validate[required,custom[integer],min[20],max[500]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->dev_call_timeout);
	fprintf(cgiOut, "					S</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"neighbor_switch_status_text\" class=\"i-title\">邻点查询使能</label>\n");
	fprintf(cgiOut, "					<div id=\"neighbor_switch_status_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"neighbor_switch_status\" name=\"neighbor_switch_status\">\n");
	fprintf(cgiOut, "							<option id=\"neighbor_switch_status_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"neighbor_switch_status_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"neighbor_period_text\" class=\"i-title\">邻点周期</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "					<div id=\"neighbor_period_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"neighbor_period\" name=\"neighbor_period\">\n");
	fprintf(cgiOut, "							<option value=\"2\">2</option>\n");
	fprintf(cgiOut, "							<option value=\"4\">4</option>\n");
	fprintf(cgiOut, "							<option value=\"6\">6</option>\n");
	fprintf(cgiOut, "							<option value=\"8\">8</option>\n");
	fprintf(cgiOut, "							<option value=\"10\">10</option>\n");
	fprintf(cgiOut, "							<option value=\"12\">12</option>\n");
	fprintf(cgiOut, "							<option value=\"14\">14</option>\n");
	fprintf(cgiOut, "							<option value=\"16\">16</option>\n");
	fprintf(cgiOut, "							<option value=\"18\">18</option>\n");
	fprintf(cgiOut, "							<option value=\"20\">20</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "					Min</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"neighbor_ai_switch_text\" class=\"i-title\">邻点上报使能</label>\n");
	fprintf(cgiOut, "					<div id=\"neighbor_ai_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"neighbor_ai_switch\" name=\"neighbor_ai_switch\">\n");
	fprintf(cgiOut, "							<option id=\"neighbor_ai_switch_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"neighbor_ai_switch_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"stop_transmit_switch_text\" class=\"i-title\">智能中转使能</label>\n");
	fprintf(cgiOut, "					<div id=\"stop_transmit_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"stop_transmit_switch\" name=\"stop_transmit_switch\">\n");
	fprintf(cgiOut, "							<option id=\"stop_transmit_switch_1\" value=\"1\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"stop_transmit_switch_2\" value=\"0\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"dev_language_switch_text\" class=\"i-title\">语言切换</label>\n");
	fprintf(cgiOut, "					<div id=\"dev_language_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"dev_language_switch\" name=\"dev_language_switch\">\n");
	fprintf(cgiOut, "							<option id=\"dev_language_switch_1\" value=\"0\">中文</option>\n");
	fprintf(cgiOut, "							<option id=\"dev_language_switch_2\" value=\"1\">英文</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"boot_mode_switch_text\" class=\"i-title\">开机方式</label>\n");
	fprintf(cgiOut, "					<div id=\"boot_mode_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"boot_mode_switch\" name=\"boot_mode_switch\">\n");
	fprintf(cgiOut, "							<option id=\"boot_mode_switch_1\" value=\"0\">按键开机</option>\n");
	fprintf(cgiOut, "							<option id=\"boot_mode_switch_2\" value=\"1\">上电自动开机</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"reboot_strategy_switch_text\" class=\"i-title\">启动策略</label>\n");
	fprintf(cgiOut, "					<div id=\"reboot_strategy_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"reboot_strategy_switch\" name=\"reboot_strategy_switch\">\n");
	fprintf(cgiOut, "							<option id=\"reboot_strategy_switch_1\" value=\"0\">快速启动</option>\n");
	fprintf(cgiOut, "							<option id=\"reboot_strategy_switch_2\" value=\"1\">全面启动</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
	fprintf(cgiOut, "			<a name=\"jcsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"alarm_text\">告警设置</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"alarm_switch_status_text\" class=\"i-title\">告警上报使能</label>\n");
	fprintf(cgiOut, "					<div id=\"alarm_switch_status_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"alarm_switch_status\" name=\"alarm_switch_status\">\n");
	fprintf(cgiOut, "							<option id=\"alarm_switch_status_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"alarm_switch_status_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"alarm_count_text\" class=\"i-title\">紧急警报重复次数</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"alarm_count\" type=\"text\" name=\"alarm_count\" value=\"%u\"  class=\"ipt-text validate[required,custom[integer],min[0],max[255]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->alarm_count);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"alarm_show_switch_text\" class=\"i-title\">紧急警报显示使能</label>\n");
	fprintf(cgiOut, "					<div id=\"alarm_show_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"alarm_show_switch\" name=\"alarm_show_switch\">\n");
	fprintf(cgiOut, "							<option id=\"alarm_show_switch_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"alarm_show_switch_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsm_alarm_show_switch_text\" class=\"i-title\">GSM告警使能</label>\n");
	fprintf(cgiOut, "					<div id=\"gsm_alarm_show_switch_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"gsm_alarm_show_switch\" name=\"gsm_alarm_show_switch\">\n");
	fprintf(cgiOut, "							<option id=\"gsm_alarm_show_switch_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"gsm_alarm_show_switch_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"radio_alarm_show_switch_text\" class=\"i-title\">中转台告警使能</label>\n");
	fprintf(cgiOut, "					<div id=\"radio_alarm_show_switch_div\" class=\"select\"  >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"radio_alarm_show_switch\" name=\"radio_alarm_show_switch\">\n");
	fprintf(cgiOut, "							<option id=\"radio_alarm_show_switch_1\" value=\"0\">关闭</option>\n");
	fprintf(cgiOut, "							<option id=\"radio_alarm_show_switch_2\" value=\"1\">开启</option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	
	fprintf(cgiOut, "	<div id=\"show3\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
	fprintf(cgiOut, "			<a name=\"zhsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"zhsz_text\">组呼设置</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team1_text\" class=\"i-title\">组1</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team1\" type=\"text\" name=\"team1\"  value=\"%s\" class=\"ipt-text validate[required,custom[team_number],min[1],max[16776415]]\"  maxlength=\"30\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\" onmouseover=\"groupNumber_mouse(1)\"   onfocus=\"groupNumber_focus(1)\" onblur=\"groupNumber_blur(1)\"  onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->group1_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team2_text\" class=\"i-title\">组2</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team2\" type=\"text\" name=\"team2\" value=\"%s\"   class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(2)\" onmouseover=\"groupNumber_mouse(2)\"  onfocus=\"groupNumber_focus(2)\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group2_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team3_text\" class=\"i-title\">组3</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team3\" type=\"text\" name=\"team3\" value=\"%s\"  class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(3)\" onmouseover=\"groupNumber_mouse(3)\" onfocus=\"groupNumber_focus(3)\" disabled=\"true\" onpaste=\"return false\"autocomplete=\"off\">\n",web_data_point->group3_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team4_text\" class=\"i-title\">组4</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team4\" type=\"text\" name=\"team4\" value=\"%s\"  class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\" data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(4)\"  onmouseover=\"groupNumber_mouse(4)\" onfocus=\"groupNumber_focus(4)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group4_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team5_text\" class=\"i-title\">组5</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team5\" type=\"text\" name=\"team5\" value=\"%s\"   class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(5)\" onfocus=\"groupNumber_focus(5)\"  onmouseover=\"groupNumber_mouse(5)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group5_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team6_text\" class=\"i-title\">组6</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team6\" type=\"text\" name=\"team6\"   value=\"%s\" class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(6)\"  onfocus=\"groupNumber_focus(6)\" onmouseover=\"groupNumber_mouse(6)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group6_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team7_text\" class=\"i-title\">组7</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team7\" type=\"text\" name=\"team7\"   value=\"%s\" class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\"  maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(7)\" onfocus=\"groupNumber_focus(7)\" onmouseover=\"groupNumber_mouse(7)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group7_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team8_text\" class=\"i-title\">组8</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team8\" type=\"text\" name=\"team8\" value=\"%s\"  class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\" maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(8)\"  onfocus=\"groupNumber_focus(8)\" onmouseover=\"groupNumber_mouse(8)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group8_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team9_text\" class=\"i-title\">组9</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team9\" type=\"text\" name=\"team9\"  value=\"%s\" class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\" maxlength=\"30\" reqmsg=\"\" onblur=\"groupNumber_blur(9)\"  onfocus=\"groupNumber_focus(9)\" onmouseover=\"groupNumber_mouse(9)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group9_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"team10_text\" class=\"i-title\">组10</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"team10\" type=\"text\" name=\"team10\" value=\"%s\"   class=\"ipt-text validate[custom[team_number],min[1],max[16776415]]\"  data-prompt-position=\"bottomRight:-112\" maxlength=\"30\" reqmsg=\"\" onmouseover=\"groupNumber_mouse(10)\"  onblur=\"groupNumber_blur(10)\"  onfocus=\"groupNumber_focus(10)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->group10_id);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"0\" id=\"team_span\">\n");
	fprintf(cgiOut, "						<input id=\"team\" type=\"text\" name=\"team\"  style=\"display:none\">\n");
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");

	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"teamIdNow_text\" class=\"i-title\">当前组</label>\n");
	fprintf(cgiOut, "					<div id=\"teamIdNow_div\" class=\"select\" >\n");
	fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
	fprintf(cgiOut, "						<select id=\"teamIdNow\" name=\"teamIdNow\" >\n");
	fprintf(cgiOut, "							<option value=\"0\">  </option>\n");
	fprintf(cgiOut, "						</select>\n");
	fprintf(cgiOut, "					</div>\n");	
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	if(web_data_point->freq_channel==1)
	{
		fprintf(cgiOut, "	<div id=\"show4\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"plsz_text\">频段350~400Mhz</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint1_text\" class=\"i-title\">频率1</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint1\" type=\"text\" name=\"frePoint1\"   value=\"%s\" class=\"ipt-text validate[required,custom[number],min[350],max[400]]\" maxlength=\"10\" data-prompt-position=\"bottomRight:-112\" onmouseover=\"freqNumber_mouse(1)\"  onblur=\"freqNumber_blur(1)\"  onfocus=\"freqNumber_focus(1)\"  onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq1);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint2_text\" class=\"i-title\">频率2</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint2\" type=\"text\" name=\"frePoint2\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(2)\"  maxlength=\"10\" onmouseover=\"freqNumber_mouse(2)\"  onfocus=\"freqNumber_focus(2)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq2);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");

		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint3_text\" class=\"i-title\">频率3</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint3\" type=\"text\" name=\"frePoint3\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(3)\"  maxlength=\"10\" onmouseover=\"freqNumber_mouse(3)\" onfocus=\"freqNumber_focus(3)\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq3);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint4_text\" class=\"i-title\">频率4</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint4\" type=\"text\" name=\"frePoint4\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(4)\" maxlength=\"10\"  onmouseover=\"freqNumber_mouse(4)\" onfocus=\"freqNumber_focus(4)\" disabled=\"true\"  onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq4);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint5_text\" class=\"i-title\">频率5</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint5\" type=\"text\" name=\"frePoint5\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\"onblur=\"freqNumber_blur(5)\" maxlength=\"10\"  onmouseover=\"freqNumber_mouse(5)\" onfocus=\"freqNumber_focus(5)\"   disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq5);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint6_text\" class=\"i-title\">频率6</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint6\" type=\"text\" name=\"frePoint6\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(6)\" maxlength=\"10\" onmouseover=\"freqNumber_mouse(6)\" onfocus=\"freqNumber_focus(6)\"  disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq6);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint7_text\" class=\"i-title\">频率7</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint7\" type=\"text\" name=\"frePoint7\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(7)\"  maxlength=\"10\" onmouseover=\"freqNumber_mouse(7)\" onfocus=\"freqNumber_focus(7)\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq7);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint8_text\" class=\"i-title\">频率8</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint8\" type=\"text\" name=\"frePoint8\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(8)\" maxlength=\"10\"  onmouseover=\"freqNumber_mouse(8)\"onfocus=\"freqNumber_focus(8)\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq8);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint9_text\" class=\"i-title\">频率9</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint9\" type=\"text\" name=\"frePoint9\" value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(9)\" maxlength=\"10\"  onmouseover=\"freqNumber_mouse(9)\" onfocus=\"freqNumber_focus(9)\" disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq9);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint10_text\"class=\"i-title\">频率10</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint10\" type=\"text\" name=\"frePoint10\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[350],max[400]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(10)\" maxlength=\"10\"  onmouseover=\"freqNumber_mouse(10)\" onfocus=\"freqNumber_focus(10)\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\" >\n",web_data_point->freq10);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"1\" id=\"fre_span\">\n");
		fprintf(cgiOut, "						<input id=\"fre\" type=\"text\" name=\"fre\" style=\"display:none\" >\n");
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePointNow_text\" class=\"i-title\">当前频率</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div id=\"frePointNow_div\" class=\"select\" >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"frePointNow\" name=\"frePointNow\" >\n");
		fprintf(cgiOut, "							<option value=\"0\">  </option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");	
		fprintf(cgiOut, "					MHz</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	}
	else if(web_data_point->freq_channel==2)
	{
		fprintf(cgiOut, "	<div id=\"show4\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"plsz_text\">频段410~470Mhz</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint1_text\" class=\"i-title\">频率1</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint1\" type=\"text\" name=\"frePoint1\"   value=\"%s\" maxlength=\"10\" class=\"ipt-text validate[required,custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\" onmouseover=\"freqNumber_mouse(1)\"  onblur=\"freqNumber_blur(1)\"  onfocus=\"freqNumber_focus(1)\"  onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq1);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint2_text\" class=\"i-title\">频率2</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint2\" type=\"text\" name=\"frePoint2\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(2)\" onmouseover=\"freqNumber_mouse(2)\"  onfocus=\"freqNumber_focus(2)\" maxlength=\"10\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq2);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");

		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint3_text\" class=\"i-title\">频率3</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint3\" type=\"text\" name=\"frePoint3\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(3)\" onmouseover=\"freqNumber_mouse(3)\"  onfocus=\"freqNumber_focus(3)\" maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq3);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint4_text\" class=\"i-title\">频率4</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint4\" type=\"text\" name=\"frePoint4\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(4)\"  onmouseover=\"freqNumber_mouse(4)\" onfocus=\"freqNumber_focus(4)\" maxlength=\"10\" disabled=\"true\"   onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq4);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint5_text\" class=\"i-title\">频率5</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint5\" type=\"text\" name=\"frePoint5\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\"onblur=\"freqNumber_blur(5)\"  onmouseover=\"freqNumber_mouse(5)\" onfocus=\"freqNumber_focus(5)\"   maxlength=\"10\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq5);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint6_text\" class=\"i-title\">频率6</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint6\" type=\"text\" name=\"frePoint6\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(6)\"  onmouseover=\"freqNumber_mouse(6)\" onfocus=\"freqNumber_focus(6)\" maxlength=\"10\"  disabled=\"true\"  onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq6);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint7_text\" class=\"i-title\">频率7</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint7\" type=\"text\" name=\"frePoint7\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(7)\"  onmouseover=\"freqNumber_mouse(7)\" onfocus=\"freqNumber_focus(7)\" maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq7);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint8_text\" class=\"i-title\">频率8</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint8\" type=\"text\" name=\"frePoint8\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\" onblur=\"freqNumber_blur(8)\" onmouseover=\"freqNumber_mouse(8)\"  onfocus=\"freqNumber_focus(8)\" maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq8);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint9_text\" class=\"i-title\">频率9</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint9\" type=\"text\" name=\"frePoint9\" value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(9)\"  onmouseover=\"freqNumber_mouse(9)\" onfocus=\"freqNumber_focus(9)\" maxlength=\"10\" disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq9);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint10_text\" class=\"i-title\">频率10</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint10\" type=\"text\" name=\"frePoint10\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[410],max[470]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"freqNumber_blur(10)\"  onmouseover=\"freqNumber_mouse(10)\" onfocus=\"freqNumber_focus(10)\" maxlength=\"10\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\" >\n",web_data_point->freq10);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"2\" id=\"fre_span\">\n");
		fprintf(cgiOut, "						<input id=\"fre\" type=\"text\" name=\"fre\" style=\"display:none\" >\n");
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePointNow_text\" class=\"i-title\">当前频率</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div id=\"frePointNow_div\" class=\"select\" >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"frePointNow\" name=\"frePointNow\" >\n");
		fprintf(cgiOut, "							<option value=\"0\">  </option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");	
		fprintf(cgiOut, "					MHz</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	}
	else if(web_data_point->freq_channel==0)
	{
		fprintf(cgiOut, "	<div id=\"show4\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"plsz_text\">无效频段</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint1_text\" class=\"i-title\">频率1</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint1\" type=\"text\" name=\"frePoint1\"   value=\"%s\" maxlength=\"10\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"  disabled=\"true\"   onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq1);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint2_text\" class=\"i-title\">频率2</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint2\" type=\"text\" name=\"frePoint2\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"  disabled=\"true\"  maxlength=\"10\"  onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq2);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");

		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint3_text\" class=\"i-title\">频率3</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint3\" type=\"text\" name=\"frePoint3\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"      maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq3);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint4_text\" class=\"i-title\">频率4</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint4\" type=\"text\" name=\"frePoint4\"   value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"   maxlength=\"10\" disabled=\"true\"   onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq4);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint5_text\" class=\"i-title\">频率5</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint5\" type=\"text\" name=\"frePoint5\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"    maxlength=\"10\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq5);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint6_text\" class=\"i-title\">频率6</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint6\" type=\"text\" name=\"frePoint6\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"    maxlength=\"10\"  disabled=\"true\"  onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq6);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint7_text\" class=\"i-title\">频率7</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint7\" type=\"text\" name=\"frePoint7\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"     maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq7);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint8_text\" class=\"i-title\">频率8</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint8\" type=\"text\" name=\"frePoint8\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"   maxlength=\"10\"  disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->freq8);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint9_text\" class=\"i-title\">频率9</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint9\" type=\"text\" name=\"frePoint9\" value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"      maxlength=\"10\" disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->freq9);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePoint10_text\" class=\"i-title\">频率10</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"frePoint10\" type=\"text\" name=\"frePoint10\"  value=\"%s\" class=\"ipt-text validate[custom[number],min[0],max[1000]]\" data-prompt-position=\"bottomRight:-112\"    maxlength=\"10\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\" >\n",web_data_point->freq10);
		fprintf(cgiOut, "					MHz</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"2\" id=\"fre_span\">\n");
		fprintf(cgiOut, "						<input id=\"fre\" type=\"text\" name=\"fre\" style=\"display:none\" >\n");
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"frePointNow_text\" class=\"i-title\">当前频率</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div id=\"frePointNow_div\" class=\"select\" >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"frePointNow\" name=\"frePointNow\" >\n");
		fprintf(cgiOut, "							<option value=\"0\">  </option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");	
		fprintf(cgiOut, "					MHz</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	}
	
	fprintf(cgiOut, "	<div id=\"show5\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
	fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"gsmComm_text\">联系人</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm1_text\" class=\"i-title\">联系人1</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm1\" type=\"text\" name=\"gsmComm1\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" maxlength=\"30\" data-prompt-position=\"bottomRight:-112\" onfocus=\"gsmNumber_focus(1)\" onmouseover=\"gsmNumber_mouse(1)\" onblur=\"gsmNumber_blur(1)\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->gsm_list1);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm2_text\" class=\"i-title\">联系人2</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm2\" type=\"text\" name=\"gsmComm2\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"gsmNumber_focus(2)\" onmouseover=\"gsmNumber_mouse(2)\" onblur=\"gsmNumber_blur(2)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->gsm_list2);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm3_text\" class=\"i-title\">联系人3</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm3\" type=\"text\" name=\"gsmComm3\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"gsmNumber_focus(3)\" onmouseover=\"gsmNumber_mouse(3)\" onblur=\"gsmNumber_blur(3)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->gsm_list3);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm4_text\" class=\"i-title\">联系人4</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm4\" type=\"text\" name=\"gsmComm4\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"gsmNumber_focus(4)\" onmouseover=\"gsmNumber_mouse(4)\" onblur=\"gsmNumber_blur(4)\" maxlength=\"30\" disabled=\"true\"  onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->gsm_list4);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm5_text\" class=\"i-title\">联系人5</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm5\" type=\"text\" name=\"gsmComm5\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"gsmNumber_focus(5)\" onmouseover=\"gsmNumber_mouse(5)\" onblur=\"gsmNumber_blur(5)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\">\n",web_data_point->gsm_list5);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm6_text\" class=\"i-title\">联系人6</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm6\" type=\"text\" name=\"gsmComm6\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"gsmNumber_focus(6)\" onmouseover=\"gsmNumber_mouse(6)\"onblur=\"gsmNumber_blur(6)\"  maxlength=\"30\" disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\" >\n",web_data_point->gsm_list6);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm7_text\" class=\"i-title\">联系人7</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm7\" type=\"text\" name=\"gsmComm7\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"gsmNumber_focus(7)\" onmouseover=\"gsmNumber_mouse(7)\" onblur=\"gsmNumber_blur(7)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\" >\n",web_data_point->gsm_list7);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm8_text\" class=\"i-title\">联系人8</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm8\" type=\"text\" name=\"gsmComm8\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"gsmNumber_focus(8)\" onmouseover=\"gsmNumber_mouse(8)\" onblur=\"gsmNumber_blur(8)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\"  autocomplete=\"off\" >\n",web_data_point->gsm_list8);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm9_text\" class=\"i-title\">联系人9</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm9\" type=\"text\" name=\"gsmComm9\"  value=\"%s\" class=\"ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"gsmNumber_focus(9)\" onmouseover=\"gsmNumber_mouse(9)\" onblur=\"gsmNumber_blur(9)\" maxlength=\"30\" disabled=\"true\" onpaste=\"return false\"   autocomplete=\"off\">\n",web_data_point->gsm_list9);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"gsmComm10_text\" class=\"i-title\">联系人10</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"gsmComm10\" type=\"text\" name=\"gsmComm10\"  value=\"%s\" class=\" ipt-text validate[custom[gsm_number],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"gsmNumber_focus(10)\" onmouseover=\"gsmNumber_mouse(10)\" onblur=\"gsmNumber_blur(10)\" maxlength=\"30\"disabled=\"true\" onpaste=\"return false\" autocomplete=\"off\" >\n",web_data_point->gsm_list10);
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"3\" id=\"gsm_span\">\n");
	fprintf(cgiOut, "						<input id=\"gsm\" type=\"text\" name=\"gsm\"  style=\"display:none\">\n");
	fprintf(cgiOut, "					</span>\n");	
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	
	
	
	fprintf(cgiOut, "	<div id=\"show6\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
    fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"update_text\">升级E-pack100</legend>\n");
	
	fprintf(cgiOut, "				<ul class=\"123\">\n");
	fprintf(cgiOut, "					<li>\n");
	fprintf(cgiOut, "						<label id=\"update1_text\" class=\"i-title\">升级文件名称</label>\n");
	fprintf(cgiOut, "						<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input type=\"text\"  name=\"upfile\" id=\"upfile\"  readonly=\"readonly\" value=\"%s\" style=\"width:200px; height:28px; border-radius: 4px;font-size: 20px;\">\n",file_name);
	fprintf(cgiOut, "						</span>\n");
	fprintf(cgiOut, "						<input type=\"file\" name =\"file\" id=\"path\" style=\"display:none\" onchange=\"file_path();\">\n");
	fprintf(cgiOut, "						<button type=\"button\"  onclick=\"path_click()\"  class=\"btn btn-primary\"><span id=\"scan\">浏览</span></button>\n");
//	fprintf(cgiOut, "					</li>\n");
//	fprintf(cgiOut, "					<li>\n");
//	fprintf(cgiOut, "						<label class=\"i-title\">升级按钮</label>\n");
	fprintf(cgiOut, "						<span class=\"ipt\">\n");
	fprintf(cgiOut, "			    		<button type=\"button\" name=\"update\" onclick=\"select_file();\" class=\"btn btn-primary\"><span id=\"update2_text\">升级</span></button>\n");
	fprintf(cgiOut, "						</span>\n");
	fprintf(cgiOut, "					</li>\n");
	fprintf(cgiOut, "				</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	fprintf(cgiOut, "	<div id=\"show7\">\n");
	fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
    fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
	fprintf(cgiOut, "			<legend id=\"version_text\">版本信息</legend>\n");
	fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"mac_text\"  class=\"i-title\">MAC地址</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"mac\" type=\"text\" name=\"mac\" value=\"%s\" class=\"ipt-text validate[maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\" style=\"background-color:#d0d0d0\">\n",web_data_point->mac);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	
	fprintf(cgiOut, "				<li>\n");
	fprintf(cgiOut, "					<label id=\"version1_text\" class=\"i-title\">E-pack100版本号</label>\n");
	fprintf(cgiOut, "					<span class=\"ipt\">\n");
	fprintf(cgiOut, "						<input id=\"version\" type=\"text\" name=\"version\" value=\"%s\" class=\"ipt-text validate[maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  readonly=\"readonly\" style=\"background-color:#d0d0d0\">\n",web_data_point->epack_version);
	fprintf(cgiOut, "					</span>\n");
	fprintf(cgiOut, "				</li>\n");
	fprintf(cgiOut, "			</ul>\n");
	fprintf(cgiOut, "		</fieldset>\n");
	fprintf(cgiOut, "	</div>\n");
	
	if(engineer_mode==NORMAL_MODE)
	{
		fprintf(cgiOut, "	<div id=\"show8\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"yhgl\"></a>\n");
		fprintf(cgiOut, "			<legend id =\"yhgl_text\">用户管理</legend>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label id =\"old_user_text\" class=\"user-label\">原用户名</label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<input type=\"text\" id =\"old_user\"  name=\"old_user\" value =\"%s\"   class=\"ipt-text validate[required,custom[onlyLetterNumber],minSize[5],maxSize[20]]\"  maxlength=\"20\" reqmsg=\"\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->old_user);
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
	
	//	fprintf(cgiOut, "				<div class=\"hr-line-dashed\"></div>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label id =\"new_user_text\" class=\"user-label\">新用户名</label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<input type=\"text\" id =\"new_user\"  name=\"new_user\"  value =\"%s\"  class=\"ipt-text validate[required,custom[onlyLetterNumber],minSize[5],maxSize[20]]\"  maxlength=\"20\" reqmsg=\"\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->new_user);
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
	
	//	fprintf(cgiOut, "				<div class=\"hr-line-dashed\"></div>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label id =\"old_code_text\" class=\"user-label\">原密码</label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<input type=\"password\" id =\"old_code\"  name=\"old_code\" value =\"%s\"  class=\"ipt-text validate[required,custom[onlyLetterNumber],minSize[5],maxSize[20]]\"  maxlength=\"20\" reqmsg=\"\" oncontextmenu=\"return false\" onpaste=\"return false\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->old_code);
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
	
	//	fprintf(cgiOut, "				<div class=\"hr-line-dashed\"></div>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label id =\"new_code_text\" class=\"user-label\">新密码</label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<input type=\"password\" id =\"new_code\" name=\"new_code\" value =\"%s\"   class=\"ipt-text validate[required,custom[onlyLetterNumber],minSize[5],maxSize[20]]\"  maxlength=\"20\" reqmsg=\"\" oncontextmenu=\"return false\" onpaste=\"return false\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->new_code);
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
	
	//	fprintf(cgiOut, "				<div class=\"hr-line-dashed\"></div>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label id =\"confirm_code_text\" class=\"user-label\">确认密码</label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<input type=\"password\" id =\"confirm_code\" name=\"confirm_code\" value =\"%s\"  class=\"ipt-text validate[required,custom[onlyLetterNumber],minSize[5],maxSize[20]]\"  maxlength=\"20\" reqmsg=\"\" oncontextmenu=\"return false\" onpaste=\"return false\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->confirm_code);
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
	
	//	fprintf(cgiOut, "				<div class=\"hr-line-dashed\"></div>\n");
		fprintf(cgiOut, "				<div class=\"form-group\">\n");
		fprintf(cgiOut, "					<label class=\"user-label\"></label>\n");
		fprintf(cgiOut, "					<div class=\"user-input\" >\n");
		fprintf(cgiOut, "						<button  class=\"user-bt\" type=\"button\" onclick=\"modify_code()\" id =\"yhgl1_text\">提　　交</button>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</div>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	}
	
	
	if(engineer_mode==ENGINEER_MODE)
	{
		fprintf(cgiOut, "	<div id=\"show8\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"blk_text\">黑名单设置</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkListConf_text\" class=\"i-title\">黑名单使能</label>\n");
		fprintf(cgiOut, "					<div id=\"blkListConf_div\" class=\"select\"  >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"blkListConf\" name=\"blkListConf\" >\n");
		fprintf(cgiOut, "							<option id=\"blkListConf_1\" value=\"0\">关闭</option>\n");
		fprintf(cgiOut, "							<option id=\"blkListConf_2\" value=\"1\">开启</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList1_text\" class=\"i-title\">黑名单1</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList1\" type=\"text\" name=\"blkList1\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" maxlength=\"30\" data-prompt-position=\"bottomRight:-112\" onmouseover=\"blackNumber_mouse(1)\" onblur=\"blackNumber_blur(1)\"  onfocus=\"blackNumber_focus(1)\" onpaste=\"return false\"  autocomplete=\"off\">\n",web_data_point->black_list1);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList2_text\" class=\"i-title\">黑名单2</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList2\" type=\"text\" name=\"blkList2\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(2)\"  onmouseover=\"blackNumber_mouse(2)\" onfocus=\"blackNumber_focus(2)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->black_list2);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList3_text\" class=\"i-title\">黑名单3</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList3\" type=\"text\" name=\"blkList3\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(3)\"  onmouseover=\"blackNumber_mouse(3)\" onfocus=\"blackNumber_focus(3)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->black_list3);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList4_text\" class=\"i-title\">黑名单4</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList4\" type=\"text\" name=\"blkList4\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(4)\"  onmouseover=\"blackNumber_mouse(4)\" onfocus=\"blackNumber_focus(4)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->black_list4);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList5_text\" class=\"i-title\">黑名单5</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList5\" type=\"text\" name=\"blkList5\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(5)\"  onmouseover=\"blackNumber_mouse(5)\" onfocus=\"blackNumber_focus(5)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->black_list5);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList6_text\" class=\"i-title\">黑名单6</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList6\" type=\"text\" name=\"blkList6\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"blackNumber_blur(6)\"  onmouseover=\"blackNumber_mouse(6)\" onfocus=\"blackNumber_focus(6)\"  maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"autocomplete=\"off\">\n",web_data_point->black_list6);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList7_text\" class=\"i-title\">黑名单7</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList7\" type=\"text\" name=\"blkList7\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(7)\"  onmouseover=\"blackNumber_mouse(7)\" onfocus=\"blackNumber_focus(7)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"  autocomplete=\"off\">\n",web_data_point->black_list7);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList8_text\" class=\"i-title\">黑名单8</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList8\" type=\"text\" name=\"blkList8\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(8)\"  onmouseover=\"blackNumber_mouse(8)\" onfocus=\"blackNumber_focus(8)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"  autocomplete=\"off\">\n",web_data_point->black_list8);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList9_text\" class=\"i-title\">黑名单9</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList9\" type=\"text\" name=\"blkList9\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onblur=\"blackNumber_blur(9)\"  onmouseover=\"blackNumber_mouse(9)\" onfocus=\"blackNumber_focus(9)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"  autocomplete=\"off\">\n",web_data_point->black_list9);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"blkList10_text\" class=\"i-title\">黑名单10</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"blkList10\" type=\"text\" name=\"blkList10\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onblur=\"blackNumber_blur(10)\"  onmouseover=\"blackNumber_mouse(10)\" onfocus=\"blackNumber_focus(10)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->black_list10);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"4\" id=\"blk_span\">\n");
		fprintf(cgiOut, "						<input id=\"blk\" type=\"text\" name=\"blk\"  style=\"display:none\">\n");
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	
	
		fprintf(cgiOut, "	<div id=\"show9\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"whtList_text\">白名单设置</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtListConf_text\" class=\"i-title\">白名单使能</label>\n");
		fprintf(cgiOut, "					<div id=\"whtListConf_div\" class=\"select\" >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"whtListConf\" name=\"whtListConf\"  >\n");
		fprintf(cgiOut, "							<option id=\"whtListConf_1\" value=\"0\">关闭</option>\n");
		fprintf(cgiOut, "							<option id=\"whtListConf_2\" value=\"1\">开启</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList1_text\" class=\"i-title\">白名单1</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList1\" type=\"text\" name=\"whtList1\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" maxlength=\"30\" data-prompt-position=\"bottomRight:-112\" onmouseover=\"whiteNumber_mouse(1)\"  onfocus=\"whiteNumber_focus(1)\" onblur=\"whiteNumber_blur(1)\"  onpaste=\"return false\" autocomplete=\"off\">\n" ,web_data_point->white_list1);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList2_text\" class=\"i-title\">白名单2</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList2\" type=\"text\" name=\"whtList2\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"whiteNumber_focus(2)\" onmouseover=\"whiteNumber_mouse(2)\" onblur=\"whiteNumber_blur(2)\"  maxlength=\"30\" onpaste=\"return false\"  disabled=\"true\" autocomplete=\"off\">\n" ,web_data_point->white_list2);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList3_text\" class=\"i-title\">白名单3</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList3\" type=\"text\" name=\"whtList3\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onfocus=\"whiteNumber_focus(3)\" onmouseover=\"whiteNumber_mouse(3)\" onblur=\"whiteNumber_blur(3)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->white_list3);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList4_text\" class=\"i-title\">白名单4</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList4\" type=\"text\" name=\"whtList4\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onfocus=\"whiteNumber_focus(4)\" onmouseover=\"whiteNumber_mouse(4)\" onblur=\"whiteNumber_blur(4)\"  maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"  autocomplete=\"off\">\n",web_data_point->white_list4);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList5_text\" class=\"i-title\">白名单5</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList5\" type=\"text\" name=\"whtList5\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onfocus=\"whiteNumber_focus(5)\" onmouseover=\"whiteNumber_mouse(5)\" onblur=\"whiteNumber_blur(5)\" maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\" autocomplete=\"off\">\n",web_data_point->white_list5);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList6_text\" class=\"i-title\">白名单6</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList6\" type=\"text\" name=\"whtList6\" value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onfocus=\"whiteNumber_focus(6)\" onmouseover=\"whiteNumber_mouse(6)\" onblur=\"whiteNumber_blur(6)\"  maxlength=\"30\" onpaste=\"return false\"  disabled=\"true\" autocomplete=\"off\">\n",web_data_point->white_list6);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList7_text\" class=\"i-title\">白名单7</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList7\" type=\"text\" name=\"whtList7\" value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"   onfocus=\"whiteNumber_focus(7)\" onmouseover=\"whiteNumber_mouse(7)\" onblur=\"whiteNumber_blur(7)\"  maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->white_list7);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList8_text\" class=\"i-title\">白名单8</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList8\" type=\"text\" name=\"whtList8\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"  onfocus=\"whiteNumber_focus(8)\" onmouseover=\"whiteNumber_mouse(8)\" onblur=\"whiteNumber_blur(8)\"  maxlength=\"30\" onpaste=\"return false\"  disabled=\"true\" autocomplete=\"off\">\n",web_data_point->white_list8);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList9_text\" class=\"i-title\">白名单9</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList9\" type=\"text\" name=\"whtList9\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\"    onfocus=\"whiteNumber_focus(9)\" onmouseover=\"whiteNumber_mouse(9)\" onblur=\"whiteNumber_blur(9)\"  maxlength=\"30\" onpaste=\"return false\" disabled=\"true\" autocomplete=\"off\">\n",web_data_point->white_list9);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"whtList10_text\" class=\"i-title\">白名单10</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"whtList10\" type=\"text\" name=\"whtList10\"  value=\"%s\" class=\"ipt-text validate[custom[integer],maxSize[30]]\" data-prompt-position=\"bottomRight:-112\" onfocus=\"whiteNumber_focus(10)\" onmouseover=\"whiteNumber_mouse(10)\" onblur=\"whiteNumber_blur(10)\"  maxlength=\"30\" onpaste=\"return false\" disabled=\"true\"  autocomplete=\"off\">\n",web_data_point->white_list10);
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		
		
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<span class=\"ipt\" tabindex=\"5\" id=\"wht_span\">\n");
		fprintf(cgiOut, "						<input id=\"wht\" type=\"text\" name=\"wht\"  style=\"display:none\">\n");
		fprintf(cgiOut, "					</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
		
		fprintf(cgiOut, "	<div id=\"show10\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"plsz\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"radio_text\" >中转台设置</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"radioIP_text\" class=\"i-title\">中转台IP</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<input id=\"radioIP\" type=\"text\" name=\"radioIP\"  value=\"%s\"   class=\"ipt-text validate[required,custom[ipv4]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->radio_ip);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"msgSlotTimePort1_text\" class=\"i-title\">短信业务时隙1端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"msgSlotTimePort1\" type=\"text\"  value=\"%u\" name=\"msgSlotTimePort1\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->busi_timeslot1);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"msgSlotTimePort2_text\" class=\"i-title\">短信业务时隙2端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"msgSlotTimePort2\" type=\"text\"  value=\"%u\" name=\"msgSlotTimePort2\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->busi_timeslot2);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"callCtrlSlotTimePort1_text\" class=\"i-title\">呼叫控制时隙1端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"callCtrlSlotTimePort1\" type=\"text\"  value=\"%u\" name=\"callCtrlSlotTimePort1\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->ctr_timeslot1);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"callCtrlSlotTimePort2_text\" class=\"i-title\">呼叫控制时隙2端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"callCtrlSlotTimePort2\" type=\"text\"   value=\"%u\" name=\"callCtrlSlotTimePort2\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->ctr_timeslot2);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"volumeSlotTimePort1_text\" class=\"i-title\">语音业务时隙1端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"volumeSlotTimePort1\" type=\"text\"  value=\"%u\" name=\"volumeSlotTimePort1\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"   autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->vol_timeslot1);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");

		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"volumeSlotTimePort2_text\" class=\"i-title\">语音业务时隙2端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"volumeSlotTimePort2\" type=\"text\"  value=\"%u\" name=\"volumeSlotTimePort2\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->vol_timeslot2);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"analogCallPort_text\" class=\"i-title\">模拟呼叫控制端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"analogCallPort\" type=\"text\"  value=\"%u\" name=\"analogCallPort\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->simulate_call_port);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"analogVolumePort_text\" class=\"i-title\">模拟语音业务端口</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"analogVolumePort\" type=\"text\"  value=\"%u\" name=\"analogVolumePort\" class=\"ipt-text validate[custom[integer],min[30000],max[65535]]\" data-prompt-position=\"bottomRight:-112\" readonly=\"readonly\"  autocomplete=\"off\" style=\"background-color:#d0d0d0\">\n",web_data_point->simulate_vol_port);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"radioID_text\" class=\"i-title\">中转台ID</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"radioID\" type=\"text\"  value=\"%u\" name=\"radioID\" class=\"ipt-text validate[custom[integer],min[1],max[16776415]]\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->radio_id);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"interConctNum_text\" class=\"i-title\">中转组</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"interConctNum\" type=\"text\"  value=\"%u\" name=\"interConctNum\" class=\"ipt-text validate[custom[integer],min[1],max[16776415]]\" data-prompt-position=\"bottomRight:-112\"   autocomplete=\"off\">\n",web_data_point->connect_group_number);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"msgCtrlAdmID_text\" class=\"i-title\">短信控制管理员ID</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"msgCtrlAdmID\" type=\"text\"  value=\"%u\" name=\"msgCtrlAdmID\" class=\"ipt-text validate[custom[integer],min[1],max[16776415]]\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->manager_id);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"msgCtrlFilter_text\" class=\"i-title\">短信控制过滤使能</label>\n");
		fprintf(cgiOut, "					<div class=\"select\"  id=\"msgCtrlFilter_div\">\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"msgCtrlFilter\" name=\"msgCtrlFilter\" >\n");
		fprintf(cgiOut, "							<option id=\"msgCtrlFilter_1\" value=\"0\">关闭</option>\n");
		fprintf(cgiOut, "							<option id=\"msgCtrlFilter_2\" value=\"1\" >开启</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
		
		
		
		fprintf(cgiOut, "	<div id=\"show11\">\n");
		fprintf(cgiOut, "		<fieldset class=\"box\" >\n");
		fprintf(cgiOut, "			<a name=\"gcms\"></a>\n");
		fprintf(cgiOut, "			<legend id=\"gcms_text\" >工程模式</legend>\n");
		fprintf(cgiOut, "			<ul class=\"ipt-box\">\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"gsm_call_interval_text\" class=\"i-title\">GSM主叫重呼间隔</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"gsm_call_interval\" type=\"text\"  value=\"%u\" name=\"gsm_call_interval\" class=\"ipt-text validate[required,custom[integer],min[10],max[60]]\" data-prompt-position=\"bottomRight:-112\"   autocomplete=\"off\">\n",web_data_point->gsm_call_interval);
		fprintf(cgiOut, "					S</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"gsm_reboot_time_text\" class=\"i-title\">GSM模块异常重启时间</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"gsm_reboot_time\" type=\"text\"  value=\"%u\" name=\"gsm_reboot_time\" class=\"ipt-text validate[required,custom[integer],min[60],max[600]]\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->gsm_reboot_time);
		fprintf(cgiOut, "					S</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"vol_start_threshold_text\" class=\"i-title\">语音检测启动门限</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div id=\"vol_start_threshold_div\" class=\"select\" >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"vol_start_threshold\" name=\"vol_start_threshold\" >\n");
		fprintf(cgiOut, "							<option value=\"20\">20</option>\n");
		fprintf(cgiOut, "							<option value=\"40\" >40</option>\n");
		fprintf(cgiOut, "							<option value=\"60\">60</option>\n");
		fprintf(cgiOut, "							<option value=\"80\" >80</option>\n");
		fprintf(cgiOut, "							<option value=\"100\">100</option>\n");
		fprintf(cgiOut, "							<option value=\"120\" >120</option>\n");
		fprintf(cgiOut, "							<option value=\"140\">140</option>\n");
		fprintf(cgiOut, "							<option value=\"160\" >160</option>\n");
		fprintf(cgiOut, "							<option value=\"180\" >180</option>\n");
		fprintf(cgiOut, "							<option value=\"200\">200</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "					Ms</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"vol_delay_text\" class=\"i-title\">语音检测拖尾延时</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"vol_delay\" type=\"text\"  value=\"%u\" name=\"vol_delay\" class=\"ipt-text validate[required,custom[integer],min[1],max[30]]\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->vol_delay);
		fprintf(cgiOut, "					S</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"vol_energy_value_text\" class=\"i-title\">语音检测能量阈值</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"vol_energy_value\" type=\"text\" value=\"%u\" name=\"vol_energy_value\" class=\"ipt-text validate[required,custom[integer],min[1500],max[4000]]\" data-prompt-position=\"bottomRight:-112\"  autocomplete=\"off\">\n",web_data_point->vol_energy_value);
		fprintf(cgiOut, "					</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"vol_packet_time_text\" class=\"i-title\">语音包时长</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div id=\"vol_packet_time_div\" class=\"select\"  >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"vol_packet_time\" name=\"vol_packet_time\" >\n");
		fprintf(cgiOut, "							<option value=\"20\">20</option>\n");
		fprintf(cgiOut, "							<option value=\"60\">60</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "					Ms</span>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"terminal_compatible_text\" class=\"i-title\">终端兼容模式</label>\n");
		fprintf(cgiOut, "					<div class=\"select\"  id=\"terminal_compatible_div\">\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"terminal_compatible\" name=\"terminal_compatible\" >\n");
		fprintf(cgiOut, "							<option id=\"terminal_compatible_1\" value=\"0\">关闭</option>\n");
		fprintf(cgiOut, "							<option id=\"terminal_compatible_2\" value=\"1\" >开启</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"work_mode_text\" class=\"i-title\">工作模式</label>\n");
		fprintf(cgiOut, "					<div id=\"work_mode_div\" class=\"select\"  >\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"work_mode\" name=\"work_mode\" >\n");
		fprintf(cgiOut, "							<option id=\"work_mode_1\" value=\"0\">PDT常规</option>\n");
		fprintf(cgiOut, "							<option id=\"work_mode_2\" value=\"1\" >DMR常规</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"voice_mode_text\" class=\"i-title\">语音编码格式</label>\n");
		fprintf(cgiOut, "					<div class=\"select\"  id=\"voice_mode_div\">\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"voice_mode\" name=\"voice_mode\" >\n");
		fprintf(cgiOut, "							<option value=\"206\">AMBE</option>\n");
		fprintf(cgiOut, "							<option value=\"207\">NVOC</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "				</li>\n");
		
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"linkTime_text\" class=\"i-title\">链路保持时间</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "					<div class=\"select\" id=\"linkTime_div\">\n");
		fprintf(cgiOut, "					<div class=\"select_mask\" ></div>\n");
		fprintf(cgiOut, "						<select id=\"linkTime\" name=\"linkTime\"  >\n");
		fprintf(cgiOut, "							<option value=\"500\">0.5</option>\n");
		fprintf(cgiOut, "							<option value=\"1000\">1.0</option>\n");
		fprintf(cgiOut, "							<option value=\"1500\">1.5</option>\n");
		fprintf(cgiOut, "							<option value=\"2000\">2.0</option>\n");
		fprintf(cgiOut, "							<option value=\"2500\">2.5</option>\n");
		fprintf(cgiOut, "							<option value=\"3000\">3.0</option>\n");
		fprintf(cgiOut, "						</select>\n");
		fprintf(cgiOut, "					</div>\n");
		fprintf(cgiOut, "					S</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"devOverTime_text\" class=\"i-title\">链路业务保持时间</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"devOverTime\" type=\"text\" name=\"devOverTime\" value=\"%u\"   class=\"ipt-text validate[required,custom[integer],min[1],max[8]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->dev_busi_time_out);
		fprintf(cgiOut, "					S</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"termGpsOverTime_text\"  class=\"i-title\">终端业务保持时间</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"termGpsOverTime\" type=\"text\" name=\"termGpsOverTime\" value=\"%u\"  class=\"ipt-text validate[required,custom[integer],min[5],max[16]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->gps_time_out);
		fprintf(cgiOut, "					S</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"close_transmit_rssi_threshold_text\" class=\"i-title\">关闭转发场强门限</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"close_transmit_rssi_threshold\" type=\"text\" name=\"close_transmit_rssi_threshold\" value=\"%d\"   class=\"ipt-text validate[required,custom[minus],min[-80],max[-40]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->close_transmit_rssi_threshold);
		fprintf(cgiOut, "					dB</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"resume_transmit_rssi_threshold_text\" class=\"i-title\">恢复转发场强门限</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"resume_transmit_rssi_threshold\" type=\"text\" name=\"resume_transmit_rssi_threshold\" value=\"%d\"   class=\"ipt-text validate[required,custom[minus],min[-90],max[-65]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->resume_transmit_rssi_threshold);
		fprintf(cgiOut, "					dB</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"tempratue_alarm_start_threshold_text\" class=\"i-title\">温度告警门限</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"tempratue_alarm_start_threshold\" type=\"text\" name=\"tempratue_alarm_start_threshold\" value=\"%u\"   class=\"ipt-text validate[required,custom[integer],min[0],max[100]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->tempratue_alarm_start_threshold);
		fprintf(cgiOut, "					℃</span>\n");	
		fprintf(cgiOut, "				</li>\n");
	
		fprintf(cgiOut, "				<li>\n");
		fprintf(cgiOut, "					<label id=\"tempratue_alarm_close_threshold_text\" class=\"i-title\">温度告警恢复门限</label>\n");
		fprintf(cgiOut, "					<span class=\"ipt\">\n");
		fprintf(cgiOut, "						<input id=\"tempratue_alarm_close_threshold\" type=\"text\" name=\"tempratue_alarm_close_threshold\" value=\"%u\" class=\"ipt-text validate[required,custom[integer],min[0],max[100]]\" data-prompt-position=\"bottomRight:-112\" maxlength=\"63\" reqmsg=\"\"  autocomplete=\"off\">\n",web_data_point->tempratue_alarm_close_threshold);
		fprintf(cgiOut, "					℃</span>\n");	
		fprintf(cgiOut, "				</li>\n");
		fprintf(cgiOut, "			</ul>\n");
		fprintf(cgiOut, "		</fieldset>\n");
		fprintf(cgiOut, "	</div>\n");
	}
	
	
	fprintf(cgiOut, "</div>\n");
	
	fprintf(cgiOut, "</form>\n");
	fprintf(cgiOut, "</body>\n");
	fprintf(cgiOut, "</html>\n");
	
}


/**
 * @init_parame
 * @获取初始化参数--默认值
 */
/*
void init_parame(WEB_NM_DATA_ITEM * web_data_point)
{
    memcpy((unsigned char *)(web_data_point->dev_ip),DEF_DEV_IP,sizeof(web_data_point->dev_ip));
    memcpy((unsigned char *)(web_data_point->subnet_mask),DEF_SUBNET_MASK,sizeof(web_data_point->subnet_mask));
    memcpy((unsigned char *)(web_data_point->def_route),DEF_ROUTE,sizeof(web_data_point->def_route));
    memcpy((unsigned char *)(web_data_point->center_ip),DEF_CENTER_IP,sizeof(web_data_point->center_ip));
    web_data_point->center_id=DEF_CENTER_ID;
    web_data_point->dev_id=DEF_DEV_ID;
    web_data_point->dev_power=DEF_POWER;
    web_data_point->dev_cc=DEF_DEV_CC;
    web_data_point->terminal_cc=DEF_TERMINAL_CC;
    web_data_point->work_mode=DEF_WORK_MODE;
    web_data_point->vol_code=DEF_VOL_CODE;
    web_data_point->locking_time=DEF_LOCKING_TIME;
    web_data_point->dev_busi_time_out=DEF_DEV_BUSI_TIMEOUT;
    web_data_point->gps_time_out=DEF_GPS_TIMEOUT;
    web_data_point->stun_start_time_out=DEF_STUN_START_TIMEOUT;
    web_data_point->half_variance_threshold=DEF_HALF_VARIANCE_THRESHOLD;
    web_data_point->mic_volume=DEF_MIC_VOLUME;
    web_data_point->gsm_volume=DEF_GSM_VOLUME;
    web_data_point->horn_volume=DEF_HORN_VOLUME;
	web_data_point->dev_call_timeout=DEF_DEV_CALL_TIMEOUT;
	web_data_point->close_transmit_threshold=DEF_CLOSE_TRANSMIT_THRESHOLD;
	web_data_point->resume_transmit_threshold=DEF_RESUME_TRANSMIT_THRESHOLD;
	web_data_point->tempratue_alarm_start_threshold=DEF_TEM_ALARM_START_THRESHOLD;
	web_data_point->tempratue_alarm_close_threshold=DEF_TEM_ALARM_CLOSE_THRESHOLD;
	web_data_point->alarm_switch_status=DEF_ALARM_SWITCH_STATUS;
	web_data_point->alarm_report_switch=DEF_PER_ALARM_SWITCH_STATUS;
	web_data_point->alarm_count=DEF_ALARM_COUNT;
	web_data_point->alarm_show_switch=DEF_ALARM_SHOW_SWITCH;
	web_data_point->gsm_alarm_show_switch=DEF_GSM_ALARM_SHOW_SWITCH;
	web_data_point->radio_alarm_show_switch=DEF_RADIO_ALARM_SHOW_SWITCH ;
	
	
    web_data_point->current_freq=DEF_CURRENT_FREQ ;
    web_data_point->blacklist_switch=DEF_BLACKLIST_SWITCH;
    web_data_point->whitelist_switch=DEF_WHITELIST_SWITCH;
    memcpy((unsigned char *)(web_data_point->radio_ip),DEF_RADIO_IP,sizeof(web_data_point->radio_ip));
    web_data_point->busi_timeslot1=DEF_BUSI_TIMESLOT1;
    web_data_point->busi_timeslot2=DEF_BUSI_TIMESLOT2;
    web_data_point->ctr_timeslot1=DEF_CTR_TIMESLOT1;
    web_data_point->ctr_timeslot2=DEF_CTR_TIMESLOT2;
    web_data_point->vol_timeslot1=DEF_VOL_BUSI_TIMESLOT1;
    web_data_point->vol_timeslot2=DEF_VOL_BUSI_TIMESLOT2;
    web_data_point->simulate_call_port=DEF_CTR_PORT;
    web_data_point->simulate_vol_port=DEF_VOL_PORT;
    web_data_point->radio_id=DEF_RADIO_ID;
    web_data_point->connect_group_number=DEF_CONNEXT_GROUP_ID;
    web_data_point->manager_id=DEF_MANAGER_ID;
    web_data_point->filter_switch=DEF_FILTER_SWITCH;
    memcpy((unsigned char *)(web_data_point->epack_version),DEF_DEV_VERSION,sizeof(web_data_point->epack_version));
    
}
*/

/**
 * @get_init_param_from_html    表单提交后，防止处理失败，获取页面配置值
 *
 * @param[out]		web_data_point	当前页面配置项值
 * @param[out]		current_div		当前显示DIV
 * @param[out]		language_switch		语言开关状态
 * @return		    engineer_mode   当前模式：常规/工程
 * @author		wdz
 * @bug
 */
int  get_init_param_from_html(WEB_NM_DATA_ITEM * web_data_point,int * current_div,int * language_switch)
{
	
	int length=0;
	int index=0;
	int tmp_value=0;
	int  engineer_mode=0;
	char temp[10]={0};
    char *power_option[]={"5","10","20"};
	char *work_mode_option[]={"0","1"};
	char *vol_code_option[]={"206","207"};
	char *locking_time_option[]={"500","1000","1500","2000","2500","3000"};
	char *half_variance_threshold_option[]={"700","1500","3000"};
	char *mic_volume_option[]={"1","2","3"};
	char *gsm_volume_option[]={"1","2","3","4","5"};
	char *horn_volume_option[]={"1","2","3","4","5"};
	char *black_list_option[]={"0","1"};
	char *white_list_option[]={"0","1"};
	char *filter_switch_option[]={"0","1"};
	char *neighbor_switch_option[]={"0","1"};
	char *neighbor_ai_switch_option[]={"0","1"};
	char *alarm_ai_switch_option[]={"0","1"};
	char *per_alarm_show_switch_option[]={"0","1"};
	char *gsm_alarm_show_switch_option[]={"0","1"};
	char *radio_alarm_show_switch_option[]={"0","1"};
	char *vol_start_threshold[]={"20","40","60","80","100","120","140","160","180","200"};
	char *neighbor_period[]={"2","4","6","8","10","12","14","16","18","20"};
	char *vol_packet_time[]={"20","60"};
	char *terminal_compatible[]={"0","1"};
	char *stop_transmit[]={"0","1"};
	char *dev_language_switch[]={"0","1"};
	char *boot_mode_switch[]={"0","1"};
	char *reboot_strategy_switch[]={"0","1"};
	
	////////////////////获取当前显示div////////////////
	cgiFormStringSpaceNeeded("current_div",&length);
	cgiFormString("current_div",temp,length);
	*current_div=atoi(temp);
	////////////////////获取当前语言////////////////
	cgiFormInteger("language_switch", language_switch,0);
	////////////////////获取当前显示模式////////////////
	cgiFormInteger("current_mode",&engineer_mode,0);
	////////////////////获取网络参数////////////////
	cgiFormStringSpaceNeeded("linkIP",&length);
	cgiFormString("linkIP",( char *)(web_data_point->dev_ip),length);
	
	cgiFormStringSpaceNeeded("mask",&length);
	cgiFormString("mask",(char *)(web_data_point->subnet_mask),length);
	
	cgiFormStringSpaceNeeded("gateWay",&length);
	cgiFormString("gateWay",(char *)(web_data_point->def_route),length);
	
	cgiFormStringSpaceNeeded("centerIP",&length);
	cgiFormString("centerIP",(char *)(web_data_point->center_ip),length);
	
	cgiFormInteger("centerID",&tmp_value,0);
	web_data_point->center_id=tmp_value;
	////////////////////获取基本配置参数////////////////
	
	cgiFormInteger("epkID",&tmp_value,0);
	web_data_point->dev_id=tmp_value;
	
	cgiFormStringSpaceNeeded("intercomID",&length);
	cgiFormString("intercomID",( char *)(&(web_data_point->dev_ai_id)),length);
	
    cgiFormSelectSingle("power",power_option,3,&index,0);
	web_data_point->dev_power=atoi(power_option[index]);
	
	cgiFormInteger("epkCode",&tmp_value,0);
	web_data_point->dev_cc=tmp_value;
	
	cgiFormInteger("termCode",&tmp_value,0);
	web_data_point->terminal_cc=tmp_value;

	cgiFormStringSpaceNeeded("workMode",&length);
	cgiFormString("workMode",(char *)(web_data_point->work_mode),length);
	
	cgiFormStringSpaceNeeded("voiceCode",&length);
	cgiFormString("voiceCode",(char *)(web_data_point->vol_code),length);
	
	

//	cgiFormInteger("termDizzyOverTime",(int *)(&(web_data_point->stun_start_time_out)),0);

	cgiFormSelectSingle("muteThreshold",half_variance_threshold_option,3,&index,0);
	web_data_point->half_variance_threshold=atoi(half_variance_threshold_option[index]);
	
	cgiFormSelectSingle("micVolume",mic_volume_option,3,&index,0);
	web_data_point->mic_volume=atoi(mic_volume_option[index]);

	cgiFormSelectSingle("gsmVolume",gsm_volume_option,5,&index,0);
	web_data_point->gsm_volume=atoi(gsm_volume_option[index]);

	cgiFormSelectSingle("trumpetVolume",horn_volume_option,5,&index,0);
	web_data_point->horn_volume=atoi(horn_volume_option[index]); 

	cgiFormInteger("devCallTimeout",&tmp_value,0);
	web_data_point->dev_call_timeout=tmp_value;
	
	cgiFormSelectSingle("neighbor_switch_status",neighbor_switch_option,2,&index,0);
	web_data_point->neighbor_switch=atoi(neighbor_switch_option[index]);
	
	cgiFormSelectSingle("neighbor_period",neighbor_period,10,&index,0);
	web_data_point->neighbor_period=atoi(neighbor_period[index]);
	
	cgiFormSelectSingle("neighbor_ai_switch",neighbor_ai_switch_option,2,&index,0);
	web_data_point->neighbor_ai_switch=atoi(neighbor_ai_switch_option[index]);
	
	cgiFormSelectSingle("stop_transmit_switch",stop_transmit,2,&index,0);
	web_data_point->stop_transmit=atoi(stop_transmit[index]);
	
	cgiFormSelectSingle("dev_language_switch",dev_language_switch,2,&index,0);
	web_data_point->device_language_switch=atoi(dev_language_switch[index]);
	
	cgiFormSelectSingle("boot_mode_switch",boot_mode_switch,2,&index,0);
	web_data_point->boot_mode_switch=atoi(boot_mode_switch[index]);
	
	cgiFormSelectSingle("reboot_strategy_switch",reboot_strategy_switch,2,&index,0);
	web_data_point->reboot_strategy_switch=atoi(reboot_strategy_switch[index]);
	
	
	cgiFormSelectSingle("alarm_switch_status",alarm_ai_switch_option,2,&index,0);
	web_data_point->alarm_ai_switch=atoi(alarm_ai_switch_option[index]);
	
	cgiFormInteger("alarm_count",&tmp_value,0);
	web_data_point->alarm_count=tmp_value;
	
	cgiFormSelectSingle("alarm_show_switch",per_alarm_show_switch_option,2,&index,0);
	web_data_point->alarm_show_switch=atoi(per_alarm_show_switch_option[index]);
	
	cgiFormSelectSingle("gsm_alarm_show_switch",gsm_alarm_show_switch_option,2,&index,0);
	web_data_point->gsm_alarm_show_switch=atoi(gsm_alarm_show_switch_option[index]);
	
	cgiFormSelectSingle("radio_alarm_show_switch",radio_alarm_show_switch_option,2,&index,0);
	web_data_point->radio_alarm_show_switch=atoi(radio_alarm_show_switch_option[index]);
	
	

	
	////////////////////获取组呼配置参数////////////////	
	
	cgiFormStringSpaceNeeded("tmp_team",&length);
	cgiFormString("tmp_team",(char *)(web_data_point->current_group_id),length);
	
	cgiFormStringSpaceNeeded("team1",&length);
	cgiFormString("team1",( char *)(web_data_point->group1_id),length);
	
	cgiFormStringSpaceNeeded("team2",&length);
	cgiFormString("team2",( char *)(web_data_point->group2_id),length);
	
	cgiFormStringSpaceNeeded("team3",&length);
	cgiFormString("team3",(char *)(web_data_point->group3_id),length);
	
	cgiFormStringSpaceNeeded("team4",&length);
	cgiFormString("team4",(char *)(web_data_point->group4_id),length);
	
	cgiFormStringSpaceNeeded("team5",&length);
	cgiFormString("team5",( char *)(web_data_point->group5_id),length);
	
	cgiFormStringSpaceNeeded("team6",&length);
	cgiFormString("team6",( char *)(web_data_point->group6_id),length);
	
	cgiFormStringSpaceNeeded("team7",&length);
	cgiFormString("team7",( char *)(web_data_point->group7_id),length);
	
	cgiFormStringSpaceNeeded("team8",&length);
	cgiFormString("team8",( char *)(web_data_point->group8_id),length);
	
	cgiFormStringSpaceNeeded("team9",&length);
	cgiFormString("team9",( char *)(web_data_point->group9_id),length);
	
	cgiFormStringSpaceNeeded("team10",&length);
	cgiFormString("team10",( char *)(web_data_point->group10_id),length);
	
	
    ////////////////////获取频率配置参数////////////////
	 memset(temp,0,sizeof(temp));
	 cgiFormStringSpaceNeeded("tmp_freq",&length);
	 cgiFormString("tmp_freq",temp,length);
	 web_data_point->current_freq=atoi(temp);
	 cgiFormStringSpaceNeeded("frePoint1",&length);
	 cgiFormString("frePoint1",( char *)(web_data_point->freq1),length);
	 cgiFormStringSpaceNeeded("frePoint2",&length);
	 cgiFormString("frePoint2",( char *)(web_data_point->freq2),length);
	 
	 cgiFormStringSpaceNeeded("frePoint3",&length);
	 cgiFormString("frePoint3",( char *)(web_data_point->freq3),length);
	 
	 cgiFormStringSpaceNeeded("frePoint4",&length);
	 cgiFormString("frePoint4",( char *)(web_data_point->freq4),length);
	 
	 cgiFormStringSpaceNeeded("frePoint5",&length);
	 cgiFormString("frePoint5",( char *)(web_data_point->freq5),length);
	 
	 cgiFormStringSpaceNeeded("frePoint6",&length);
	 cgiFormString("frePoint6",( char *)(web_data_point->freq6),length);
	 cgiFormStringSpaceNeeded("frePoint7",&length);
	 cgiFormString("frePoint7",(char *)(web_data_point->freq7),length);
	 cgiFormStringSpaceNeeded("frePoint8",&length);
	 cgiFormString("frePoint8",( char *)(web_data_point->freq8),length);
	 cgiFormStringSpaceNeeded("frePoint9",&length);
	 cgiFormString("frePoint9",( char *)(web_data_point->freq9),length);
	 cgiFormStringSpaceNeeded("frePoint10",&length);
	 cgiFormString("frePoint10",( char *)(web_data_point->freq10),length);
	 
	 
	 ////////////////////获取gsm配置参数////////////////
	 cgiFormStringSpaceNeeded("gsmComm1",&length);
	 cgiFormString("gsmComm1",( char *)(web_data_point->gsm_list1),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm2",&length);
	 cgiFormString("gsmComm2",(char *)(web_data_point->gsm_list2),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm3",&length);
	 cgiFormString("gsmComm3",( char *)(web_data_point->gsm_list3),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm4",&length);
	 cgiFormString("gsmComm4",(char *)(web_data_point->gsm_list4),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm5",&length);
	 cgiFormString("gsmComm5",(char *)(web_data_point->gsm_list5),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm6",&length);
	 cgiFormString("gsmComm6",( char *)(web_data_point->gsm_list6),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm7",&length);
	 cgiFormString("gsmComm7",(char *)(web_data_point->gsm_list7),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm8",&length);
	 cgiFormString("gsmComm8",(char *)(web_data_point->gsm_list8),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm9",&length);
	 cgiFormString("gsmComm9",(char *)(web_data_point->gsm_list9),length);
	 
	 cgiFormStringSpaceNeeded("gsmComm10",&length);
	 cgiFormString("gsmComm10",(char *)(web_data_point->gsm_list10),length);
	 
	 ////////////////////获取版本信息参数////////////////
	 cgiFormStringSpaceNeeded("version",&length);
	 cgiFormString("version",(char *)(web_data_point->epack_version),length);
	 
	 cgiFormStringSpaceNeeded("mac",&length);
	 cgiFormString("mac",(char *)(web_data_point->mac),length);
	 ////////////////////获取工程模式参数////////////////
	 if(engineer_mode==ENGINEER_MODE)
	 {
		 ////////////////////获取黑名单配置参数////////////////
	 
		cgiFormSelectSingle("blkListConf",black_list_option,2,&index,0);
		web_data_point->blacklist_switch=atoi(black_list_option[index]);
	 
		cgiFormStringSpaceNeeded("blkList1",&length);
		cgiFormString("blkList1",(char *)(web_data_point->black_list1),length); 
	 
		cgiFormStringSpaceNeeded("blkList2",&length);
		cgiFormString("blkList2",(char *)(web_data_point->black_list2),length); 
	 
		cgiFormStringSpaceNeeded("blkList3",&length);
		cgiFormString("blkList3",( char *)(web_data_point->black_list3),length); 
	 
		cgiFormStringSpaceNeeded("blkList4",&length);
		cgiFormString("blkList4",( char *)(web_data_point->black_list4),length); 
	 
		cgiFormStringSpaceNeeded("blkList5",&length);
		cgiFormString("blkList5",( char *)(web_data_point->black_list5),length); 
	 
		cgiFormStringSpaceNeeded("blkList6",&length);
		cgiFormString("blkList6",( char *)(web_data_point->black_list6),length); 
	 
		cgiFormStringSpaceNeeded("blkList7",&length);
		cgiFormString("blkList7",( char *)(web_data_point->black_list7),length); 
	 
		cgiFormStringSpaceNeeded("blkList8",&length);
		cgiFormString("blkList8",( char *)(web_data_point->black_list8),length); 
	 
		cgiFormStringSpaceNeeded("blkList9",&length);
		cgiFormString("blkList9",(char *)(web_data_point->black_list9),length); 
	 
		cgiFormStringSpaceNeeded("blkList10",&length);
		cgiFormString("blkList10",(char *)(web_data_point->black_list10),length);

////////////////////获取白名单配置参数////////////////
	 
		cgiFormSelectSingle("whtListConf",white_list_option,2,&index,0);
		web_data_point->whitelist_switch=atoi(white_list_option[index]);
	 
		cgiFormStringSpaceNeeded("whtList1",&length);
		cgiFormString("whtList1",(char *)(web_data_point->white_list1),length); 	 
	 
		cgiFormStringSpaceNeeded("whtList2",&length);
		cgiFormString("whtList2",(char *)(web_data_point->white_list2),length); 
		
		cgiFormStringSpaceNeeded("whtList3",&length);
		cgiFormString("whtList3",(char *)(web_data_point->white_list3),length); 
	 
		cgiFormStringSpaceNeeded("whtList4",&length);
		cgiFormString("whtList4",(char *)(web_data_point->white_list4),length); 
	 
		cgiFormStringSpaceNeeded("whtList5",&length);
		cgiFormString("whtList5",(char *)(web_data_point->white_list5),length); 
	 
		cgiFormStringSpaceNeeded("whtList6",&length);
		cgiFormString("whtList6",( char *)(web_data_point->white_list6),length); 
	 
		cgiFormStringSpaceNeeded("whtList7",&length);
		cgiFormString("whtList7",(char *)(web_data_point->white_list7),length); 
	 
		cgiFormStringSpaceNeeded("whtList8",&length);
		cgiFormString("whtList8",(char *)(web_data_point->white_list8),length); 
	 
		cgiFormStringSpaceNeeded("whtList9",&length);
		cgiFormString("whtList9",(char *)(web_data_point->white_list9),length); 
	 
		cgiFormStringSpaceNeeded("whtList10",&length);
		cgiFormString("whtList10",(char *)(web_data_point->white_list10),length);
		
		cgiFormSelectSingle("linkTime",locking_time_option,6,&index,0);
		web_data_point->locking_time=atoi(locking_time_option[index]);
	
		cgiFormInteger("devOverTime",&tmp_value,0);
		web_data_point->dev_busi_time_out=tmp_value;
	
		cgiFormInteger("termGpsOverTime",&tmp_value,0);
		web_data_point->gps_time_out=tmp_value;
		
		cgiFormInteger("tempratue_alarm_start_threshold",&tmp_value,0);
		web_data_point->tempratue_alarm_start_threshold=tmp_value;
	
		cgiFormInteger("tempratue_alarm_close_threshold",&tmp_value,0);
		web_data_point->tempratue_alarm_close_threshold=tmp_value;
		
		cgiFormInteger("close_transmit_rssi_threshold",&tmp_value,0);
		web_data_point->close_transmit_rssi_threshold=tmp_value;
		
		cgiFormInteger("resume_transmit_rssi_threshold",&tmp_value,0);
		web_data_point->resume_transmit_rssi_threshold=tmp_value;
	
		cgiFormInteger("gsm_call_interval",&tmp_value,0);
		web_data_point->gsm_call_interval=tmp_value;
		
		cgiFormInteger("gsm_reboot_time",&tmp_value,0);
		web_data_point->gsm_reboot_time=tmp_value;
	 
		cgiFormSelectSingle("vol_start_threshold", vol_start_threshold,10,&index,0);
		web_data_point->vol_start_threshold=atoi(vol_start_threshold[index]);
	
		cgiFormInteger("vol_delay",&tmp_value,0);
		web_data_point->vol_delay=tmp_value;
	
		cgiFormInteger("vol_energy_value",&tmp_value,0);
		web_data_point->vol_energy_value=tmp_value;
	
		cgiFormSelectSingle("vol_packet_time",vol_packet_time,2,&index,0);
		web_data_point->vol_packet_time=atoi( vol_packet_time[index]);
	
		cgiFormSelectSingle("terminal_compatible",terminal_compatible,2,&index,0);
		web_data_point->terminal_compatible=atoi(terminal_compatible[index]);
		
		////////////////////获取中转台配置参数////////////////
		cgiFormStringSpaceNeeded("radioIP",&length);
		cgiFormString("radioIP",(char *)(web_data_point->radio_ip),length);
	 
		cgiFormInteger("msgSlotTimePort1",&tmp_value,0);
		web_data_point->busi_timeslot1=tmp_value;
		
		cgiFormInteger("msgSlotTimePort2",&tmp_value,0);
		web_data_point->busi_timeslot2=tmp_value;
	  
		cgiFormInteger("callCtrlSlotTimePort1",&tmp_value,0);
		web_data_point->ctr_timeslot1=tmp_value;

		cgiFormInteger("callCtrlSlotTimePort2",&tmp_value,0);
		web_data_point->ctr_timeslot2=tmp_value;
		
		cgiFormInteger("volumeSlotTimePort1",&tmp_value,0);
		web_data_point->vol_timeslot1=tmp_value;
		
		cgiFormInteger("volumeSlotTimePort2",&tmp_value,0);
		web_data_point->vol_timeslot2=tmp_value;

		cgiFormInteger("analogCallPort",&tmp_value,0);
		web_data_point->simulate_call_port=tmp_value;
		
		cgiFormInteger("analogVolumePort",&tmp_value,0);
		web_data_point->simulate_vol_port=tmp_value;
		
		cgiFormInteger("radioID",&tmp_value,0);
		web_data_point->radio_id=tmp_value;
		
		cgiFormInteger("interConctNum",&tmp_value,0);
		web_data_point->connect_group_number=tmp_value;
		
		cgiFormInteger("msgCtrlAdmID",&tmp_value,0);
		web_data_point->manager_id=tmp_value;

		cgiFormSelectSingle("msgCtrlFilter",filter_switch_option,2,&index,0);
		web_data_point->filter_switch=atoi(filter_switch_option[index]); 
		
		cgiFormSelectSingle("work_mode",work_mode_option,2,&index,0);
		web_data_point->work_mode_2=atoi(work_mode_option[index]);
	 
		cgiFormSelectSingle("voice_mode",vol_code_option,2,&index,0);
		web_data_point->vol_code_2=atoi(vol_code_option[index]);
	 }
	 else if(engineer_mode==NORMAL_MODE)
	 {
		 cgiFormStringSpaceNeeded("old_user",&length);
		 cgiFormString("old_user",( char *)(web_data_point->old_user),length); 
		 
		 cgiFormStringSpaceNeeded("new_user",&length);
		 cgiFormString("new_user",(char *)(web_data_point->new_user),length);
		 
		 cgiFormStringSpaceNeeded("old_code",&length);
		 cgiFormString("old_code",( char *)(web_data_point->old_code),length); 
		 
		 cgiFormStringSpaceNeeded("new_code",&length);
		 cgiFormString("new_code",(char *)(web_data_point->new_code),length);
		 
		 cgiFormStringSpaceNeeded("confirm_code",&length);
		 cgiFormString("confirm_code",(char *)(web_data_point->confirm_code),length);
		 
		 ////////////////////获取保存的用户名和密码////////////////
		cgiFormStringSpaceNeeded("web_user",&length);
		cgiFormString("web_user",( char *)(web_data_point->web_user),length);
	
		cgiFormStringSpaceNeeded(( char *)("web_code"),&length);
		cgiFormString("web_code",(char *)(web_data_point->web_code),length);
	 }
	 return engineer_mode;
	 
}
/**
 * @shm_cfg_attch
 * @打开配置区共享内存和信号量
 */
int shm_cfg_attch(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_CFG);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_CFG);  

	shmid_cfg = shmget(shmkey, 0, 0); //获取共享内存 
	if (-1 == shmid_cfg)  
	{
		system("echo shm_cfg_attch shmget fail > /tmp/web_error.txt");
		return -1;
	}

	shm_cfg_addr = (SHM_CFG_STRU *)shmat(shmid_cfg, NULL, 0);
	if (-1 == (int)(shm_cfg_addr))
	{
		system("echo shm_cfg_attch shmat fail > /tmp/web_error.txt");
		return -1;
	}

	semid_cfg = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_cfg) 
	{
		system("echo shm_cfg_attch semget fail > /tmp/web_error.txt");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_cfg, 0, SETVAL, sem_arg)) 
    {
		system("echo shm_cfg_attch semctl fail > /tmp/web_error.txt");
		return -1;
	}

	return 0;
}
/**
 * @sem_cfg_p
 * @获取配置区共享内存的信号量
 */
void sem_cfg_p(void)
{
	struct sembuf s_p;
	s_p.sem_num = 0;
	s_p.sem_op = -1;
	s_p.sem_flg = SEM_UNDO;
	if (-1 == semop(semid_cfg, &s_p, 1))
	{
		system("echo sem_cfg_p fail > /tmp/web_error.txt");
	}
}

 /**
 * @sem_cfg_v
 * @释放配置区共享内存的信号量
 */
void sem_cfg_v(void)
{
	struct sembuf s_v;
	s_v.sem_num = 0;
	s_v.sem_op = 1;
	s_v.sem_flg = SEM_UNDO;
	if(-1 == semop(semid_cfg, &s_v, 1))
	{
		system("echo sem_cfg_v fail > /tmp/web_error.txt");
	}
}



/**
 * @save_ini_file
 * @保存配置文件
 */
int  save_ini_file(void)
{
	int fd;	
	chdir("/loadapp");
    fd = open(NAS_CONFIG_FILE, O_RDWR);
	if (-1 == fd)
	{
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	sem_cfg_p();
	write(fd, shm_cfg_addr, sizeof(SHM_CFG_STRU));
	sem_cfg_v();
	close(fd);
	return 0;
}
/**
 * @mgrh_msg_queue_create
 * @打开mgr_local_handle函数创建的消息队列
 */
int mgrh_msg_queue_create(void)
{
	key_t key;

	key = ftok(FTOK_F_NAME, FTOK_ID_Q_MGRH);
	qid_mgrh = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
	if (-1 == qid_mgrh)
	{
		system("echo mgrh_msg_queue_create fail > /tmp/web_error.txt");
		return -1;
	}

	return 0;
}
/**
 * @send_reboot_to_queque
 * @异步传送命令给mgr_local_handle函数创建的消息队列
 */
void send_cmd_to_queque(unsigned short cmd)
{
    QUEUE_MSG q_msg;  
	q_msg.mtype = MSG_TYPE_CMD;

	q_msg.ipc_msg.dst_id=0;
	q_msg.ipc_msg.src_id=0;
	q_msg.ipc_msg.nm_type=NM_TYPE_NM;
	q_msg.ipc_msg.cmd_code=cmd;
	q_msg.ipc_msg.op_code=OP_CODE_SET;

	if(-1 ==msgsnd(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0))
	{
		system("echo send cmd to  msg_queue fail > /tmp/web_error.txt");
	}

}
/**
 * @eeprom_read
 * @读取eeprom
 */
int  eeprom_read(int readfd,unsigned short eepadd, unsigned char *data_read,unsigned int length)
{ 
	struct i2c_msg msg_read[2];
	struct i2c_rdwr_ioctl_data rdwr_arg; 
	
	//printf("\neeprom read!\n\n"); 
	msg_read[0].addr = 0x50;
	msg_read[0].flags = 0;
	
	buf_read[0] = (unsigned char)((eepadd & 0xFF00)>>8);//addr_h
	buf_read[1] = (unsigned char)(eepadd & 0x00FF);//addr_l
	msg_read[0].buf = buf_read;
	msg_read[0].len = 2;
	
	msg_read[1].addr = 0x50;
	msg_read[1].flags = I2C_M_RD;
	msg_read[1].buf = data_read;
	msg_read[1].len = length;
	
	rdwr_arg.msgs = msg_read;
	rdwr_arg.nmsgs = 2;
	if(ioctl(readfd,I2C_RDWR,(unsigned int )&rdwr_arg) < 0)
	{	
		system("echo eeprom_read error > /tmp/web_error.txt");
		return -1;
	}
	
	return 0;
}
/**
 * @init_eeprom
 * @初始化eeprom
 */
int init_eeprom(void)
{
 	eeprom_fd = open("/dev/i2c-0",O_RDWR);
	if (eeprom_fd < 0)
	{
		system("echo open /dev/i2c fail > /tmp/web_error.txt"); 
        return -1;
	}
	return 0;
}


/**
 * @read_esn_param_from_eeprom
 * @读取mac
 */

int read_esn_param_from_eeprom(void)
{
	if (init_eeprom())
	{
		return -1;
	}
	if (eeprom_read(eeprom_fd, 0x4100, (unsigned char *)&eeprom_esn_param, sizeof(EEPROM_ESN_PARAM)) == -1)
	{
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	return 0;
}



/**
 * @get_data_from_file
 * @从配置文件读取配置信息
 */
int get_data_from_file()
{
	int fd=0;
	int i=0;
	int language_switch=0;
	unsigned short vol_code;
	unsigned short work_mode;
	double tmp_freq=0;
	unsigned char *p=NULL;
	unsigned char *q=NULL;
	unsigned int temp;
	struct in_addr addr1;
	char temp_data[10]={0};
	char vol_code_a[10]="AMBE";
	char vol_code_n[10]="NVOC";
	char work_mode_p[10]="PDT常规";
	char work_mode_d[10]="DMR常规";
	char work_mode_p_en[50]="PDT Conventional";
	char work_mode_d_en[50]="DMR Conventional";
	
	//////////////////获取语言开关///////////////
	cgiFormInteger("language_switch", &language_switch,0);
	
	fd = open(NM_CENTER_CONFIG_FILE, O_RDWR);
	if(-1==fd)
	{
		system("echo open Configfile fail > /tmp/web_error.txt"); 
		return -1;
	}
	CENTER_DATA_STRUCT  center_struct;
	memset((unsigned char *)&center_struct,0,sizeof(center_struct));
	lseek(fd,0,SEEK_SET);
	
	if(sizeof(center_struct)!=read(fd,(unsigned char*)&center_struct,sizeof(center_struct)))
	{
		system("echo read Configfile fail > /tmp/web_error.txt"); 
		close(fd);
		return -1;
	}
	close(fd);
	memset((unsigned char *)&web_data,0,sizeof(web_data));
	
	//////////中心IP/////////////////
	temp=htonl(center_struct.center_ip);
	memcpy(&addr1,(char *)&temp,sizeof(temp));
	memcpy((char *)(web_data.center_ip),inet_ntoa(addr1),strlen(inet_ntoa(addr1)));
	//////////中心ID/////////////////
	web_data.center_id=center_struct.center_id;
	//////////对讲ID/////////////////
	memcpy((char *)(web_data.dev_ai_id),center_struct.dev_ai_id,sizeof(center_struct.dev_ai_id));
	//////////语音编码格式/////////////////
	vol_code=center_struct.vol_code;
	if(0xce == vol_code )
	{
		memcpy(web_data.vol_code,vol_code_a,sizeof(vol_code_a));
	}
	else if(0xcf == vol_code)
	{
		memcpy(web_data.vol_code,vol_code_n,sizeof(vol_code_n));
	}
	///////////////工程模式///////////////
	web_data.vol_code_2=vol_code;
	web_data.gsm_call_interval=center_struct.gsm_call_interval;
	web_data.gsm_reboot_time=center_struct.gsm_reboot_time;
	web_data.vol_start_threshold=center_struct.vol_start_threshold;
	web_data.vol_delay=center_struct.vol_delay;
	web_data.vol_energy_value=center_struct.vol_energy_value;
	web_data.vol_packet_time=center_struct.vol_packet_time;
	////////////////工程模式///////////////
	
	
	//////////MIC音量/////////////////
	web_data.mic_volume=center_struct.mic_volume;
	//////////GSM音量/////////////////
	web_data.gsm_volume=center_struct.gsm_volume;
	//////////喇叭音量/////////////////
	web_data.horn_volume=center_struct.horn_volume;
	//////////语言开关/////////////////
	web_data.device_language_switch=center_struct.dev_language_switch;
	//////////人员告警上报次数/////////////////
	web_data.alarm_count=center_struct.alarm_count;
	//////////人员告警显示开关/////////////////
	web_data.alarm_show_switch=center_struct.alarm_display;
	//////////gsm、radio告警显示开关/////////////////
	if(center_struct.dev_alarm_icon_show_switch[0]&0x10)
	{
		web_data.gsm_alarm_show_switch=1;
	}
	else
	{
		web_data.gsm_alarm_show_switch=0;
	}
	
	if(center_struct.dev_alarm_icon_show_switch[1]&0x04)
	{
		web_data.radio_alarm_show_switch=1;
	}
	else
	{
		web_data.radio_alarm_show_switch=0;
	}
	
	
	//////////当前组ID/////////////////
	memcpy((char *)web_data.current_group_id,center_struct.current_group_id,sizeof(center_struct.current_group_id));
//////////组呼列表/////////////////
	p=(unsigned char *)(web_data.group1_id);
	q=(unsigned char *)(center_struct.group1_id);
	for(i=0;i<10;i++)
	{
		if(q[0]==0)
		{
			break;
		}
		else
		{
			memcpy(p,q,WEB_STRING_MAX_LENGTH);
			p+=WEB_STRING_MAX_LENGTH;
			q+=WEB_STRING_MAX_LENGTH;
		}
	}
	//////////频点列表/////////////////
	p=(unsigned  char *)(web_data.freq1);
	q=(unsigned char *)(center_struct.freq1);
	for(i=0;i<10;i++)
	{
		if(q[0]==0)
		{
			break;
		}
		else
		{
			memcpy(temp_data,q,sizeof(temp_data));
			tmp_freq=atoi(temp_data)/1000000.0;
			memset(p,0,11);
			sprintf((char *)p,"%1.6f",tmp_freq);
			p+=11;
			q+=10;
			
		}
	}
	//////////黑名单开关/////////////////
	web_data.blacklist_switch=center_struct.blacklist_switch;
	//////////黑名单列表/////////////////
	p=( unsigned char *)(web_data.black_list1);
	q=( unsigned char *)(center_struct.black_list1);
	for(i=0;i<10;i++)
	{
		if(q[0]==0)
		{
			break;
		}
		else
		{
			memcpy(p,q,WEB_STRING_MAX_LENGTH);
			p+=(WEB_STRING_MAX_LENGTH+1);
			q+=WEB_STRING_MAX_LENGTH;
		}
	}
	
	//////////白名单开关/////////////////
	web_data.whitelist_switch=center_struct.whitelist_switch;
	//////////白名单列表/////////////////
	p=(unsigned char *)(web_data.white_list1);
	q=( unsigned char *)(center_struct.white_list1);
	for(i=0;i<10;i++)
	{
		if(q[0]==0)
		{
			break;
		}
		else
		{
			memcpy(p,q,WEB_STRING_MAX_LENGTH);
			p+=(WEB_STRING_MAX_LENGTH+1);
			q+=WEB_STRING_MAX_LENGTH;
		}
	}
	
	//////////gsm列表/////////////////
	p=(unsigned char *)(web_data.gsm_list1);
	q=(unsigned char *)(center_struct.gsm_list1);
	for(i=0;i<10;i++)
	{
		if(q[0]==0)
		{
			break;
		}
		else
		{
			memcpy(p,q,WEB_STRING_MAX_LENGTH);
			p+=(WEB_STRING_MAX_LENGTH+1);
			q+=WEB_STRING_MAX_LENGTH;
		}
	}
	//////////中转台列表/////////////////
	
	temp=htonl(center_struct.radio_ip);
	memcpy(&addr1,( char *)&temp,sizeof(temp));
	memcpy((char *)(web_data.radio_ip),inet_ntoa(addr1),strlen(inet_ntoa(addr1)));
	
	
	web_data.busi_timeslot1=center_struct.busi_timeslot1;
	web_data.busi_timeslot2=center_struct.busi_timeslot2;
	web_data.ctr_timeslot1=center_struct.ctr_timeslot1;
	web_data.ctr_timeslot2=center_struct.ctr_timeslot2;
	web_data.vol_timeslot1=center_struct.vol_timeslot1;
	web_data.vol_timeslot2=center_struct.vol_timeslot2;
	web_data.simulate_call_port=center_struct.simulate_call_port;
	web_data.simulate_vol_port=center_struct.simulate_vol_port;
	web_data.radio_id=center_struct.radio_id;
	web_data.connect_group_number=center_struct.connect_group_number;
	web_data.manager_id=center_struct.manager_id;
	web_data.filter_switch=center_struct.filter_switch;
	
	//////////////////////共享内存读取数据/////////////////
	memcpy(web_data.dev_ip,shm_cfg_addr->local_ip.buf,sizeof(web_data.dev_ip));
	memcpy(web_data.subnet_mask,shm_cfg_addr->subnet_mask.buf,sizeof(web_data.subnet_mask));
	memcpy(web_data.def_route,shm_cfg_addr->gateway.buf,sizeof(web_data.def_route));
	web_data.dev_id=shm_cfg_addr->dev_id.val+1;
	web_data.dev_power=shm_cfg_addr->power.val;
	web_data.dev_cc=shm_cfg_addr->cc.val;
	web_data.terminal_cc=shm_cfg_addr->terminal_cc.val;
	work_mode=shm_cfg_addr->protocol_mode.val;
	
	if(language_switch==0)
	{
		if(0==work_mode)
		{
			memcpy(web_data.work_mode,work_mode_p,sizeof(work_mode_p));
		}
		else if(1== work_mode)
		{
			memcpy(web_data.work_mode,work_mode_d,sizeof(work_mode_d));
		}
	}
	else
	{
		if(0==work_mode)
		{
			memcpy(web_data.work_mode,work_mode_p_en,sizeof(work_mode_p_en));
		}
		else if(1== work_mode)
		{
			memcpy(web_data.work_mode,work_mode_d_en,sizeof(work_mode_d_en));
		}
	}
	
	//////////////////工程模式///////////////
	web_data.work_mode_2=work_mode;
	web_data.terminal_compatible=shm_cfg_addr->moto_mode_switch.val;
	//////////////////工程模式///////////////
	web_data.locking_time=shm_cfg_addr->locking_time.val;
	web_data.dev_busi_time_out=shm_cfg_addr->work_time_out.val/1000;
	web_data.gps_time_out=shm_cfg_addr->gps_time_out.val/1000;
//	web_data.stun_start_time_out=shm_cfg_addr->stun_time_out.val/1000;
	web_data.half_variance_threshold=shm_cfg_addr->half_variance_threshold.val;
	web_data.current_freq=shm_cfg_addr->freq.val;
	memcpy(web_data.epack_version,shm_cfg_addr->s_version.buf,sizeof(web_data.epack_version));
	
	if(0xce == vol_code )
	{
		web_data.epack_version[0]='A';
	}
	else if(0xcf == vol_code)
	{
		web_data.epack_version[0]='N';
	}
	
	web_data.dev_call_timeout=shm_cfg_addr->dev_call_timeout.val;
	web_data.neighbor_switch=shm_cfg_addr->start_neighbor.val;
	web_data.neighbor_period=shm_cfg_addr->neighbor_period.val*2;
	web_data.neighbor_ai_switch=shm_cfg_addr->neighbor_report_ai.val;
	web_data.stop_transmit=shm_cfg_addr->stop_tans.val;
	web_data.boot_mode_switch=shm_cfg_addr->boot_mode.val;
	web_data.reboot_strategy_switch=shm_cfg_addr->reboot_strategy.val;
	web_data.tempratue_alarm_start_threshold=shm_cfg_addr->tempratue_alarm_start_threshold.val;
	web_data.tempratue_alarm_close_threshold=shm_cfg_addr->tempratue_alarm_close_threshold.val;
	web_data.alarm_ai_switch=shm_cfg_addr->alarm_switch_status.val;
	web_data.close_transmit_rssi_threshold=(-1)*(shm_cfg_addr->close_transmit_threshold.val);
	web_data.resume_transmit_rssi_threshold=(-1)*(shm_cfg_addr->resume_transmit_threshold.val);
	memset((unsigned char *)&eeprom_esn_param,0,sizeof(EEPROM_ESN_PARAM));
	if (read_esn_param_from_eeprom())
	{
		return -1 ;
	}
	sprintf(web_data.mac,"%x:%x:%x:%x:%x:%x",eeprom_esn_param.mac_addr[0],eeprom_esn_param.mac_addr[1],eeprom_esn_param.mac_addr[2],\
							                                eeprom_esn_param.mac_addr[3],eeprom_esn_param.mac_addr[4],eeprom_esn_param.mac_addr[5]);
	for(i=0;web_data.mac[i]!='\0';i++)
	{
		if(web_data.mac[i]<='z'&&web_data.mac[i]>='a') 
			web_data.mac[i]-=32;
	}
	
	return 0;
}




/**
 * @set_data_to_file    修改配置
 *
 * @param[in]		web_set_data	保存配置项结构体
 * @return		    函数执行状态
 * @author		wdz
 * @bug
 */
int set_data_to_file(WEB_NM_DATA_ITEM * web_set_data)
{
	int fd=0;
	int i=0;
	int engineer_mode=0;
	unsigned int temp;
    unsigned char * p=NULL;
	unsigned char * q=NULL;
	char tmp_freq[10]={0};
	fd = open(NM_CENTER_CONFIG_FILE, O_RDWR);
	if(-1==fd)
	{
		system("echo open Configfile fail > /tmp/web_error.txt"); 
		return -1;
	}
	CENTER_DATA_STRUCT  center_struct;
	memset((unsigned char *)&center_struct,0,sizeof(center_struct));
	lseek(fd,0,SEEK_SET);
	
	if(sizeof(center_struct)!=read(fd,(unsigned char*)&center_struct,sizeof(center_struct)))
	{
		system("echo read Configfile fail > /tmp/web_error.txt"); 
		close(fd);
		return -1;
	}
	
	cgiFormInteger("current_mode",&engineer_mode,0);
	///////////////////////配置中心文件////////////////////////
	//////////中心IP/////////////////
	temp=inet_addr( web_set_data->center_ip);
	temp=ntohl(temp);
	center_struct.center_ip=temp;
	//////////中心ID/////////////////
	center_struct.center_id=web_set_data->center_id;
	//////////对讲ID/////////////////
	memcpy(center_struct.dev_ai_id,web_set_data->dev_ai_id,sizeof(center_struct.dev_ai_id));
	
	
	//////////工程模式/////////////////
	if(engineer_mode==ENGINEER_MODE)
	{
		center_struct.vol_code=web_set_data->vol_code_2;
		center_struct.gsm_call_interval=web_set_data->gsm_call_interval;
		center_struct.gsm_reboot_time=web_set_data->gsm_reboot_time;
		center_struct.vol_start_threshold=web_set_data->vol_start_threshold;
		center_struct.vol_delay=web_set_data->vol_delay;
		center_struct.vol_energy_value=web_set_data->vol_energy_value;
		center_struct.vol_packet_time=web_set_data->vol_packet_time;
	}
	
	//////////工程模式/////////////////
	
	//////////MIC音量/////////////////
	center_struct.mic_volume=web_set_data->mic_volume;
	//////////GSM音量/////////////////
	center_struct.gsm_volume=web_set_data->gsm_volume;
	//////////喇叭音量/////////////////
	center_struct.horn_volume=web_set_data->horn_volume;
	//////////语言开关/////////////////
	center_struct.dev_language_switch=web_set_data->device_language_switch;
	//////////当前组ID/////////////////
	memcpy(center_struct.current_group_id,web_set_data->current_group_id,sizeof(center_struct.current_group_id));
	center_struct.alarm_group=atoi(center_struct.current_group_id);
	//////////组列表/////////////////
	p=( unsigned char *)(web_set_data->group1_id);
	q=( unsigned char *)(center_struct.group1_id);
	memcpy(q,p,sizeof(web_set_data->group1_id)*10);
   //////////频点列表/////////////////
    p=( unsigned char *)(web_set_data->freq1);
	q=(unsigned char *)(center_struct.freq1);
	memset(q,0,sizeof(center_struct.freq1)*10);
	for( i=0;i<10;i++)
	{
		if(p[0]==0)
		{
			break;
		}
		else
			
		{
			memcpy(tmp_freq,p,10);
			temp=atof(tmp_freq)*1000000;
			memset(q,0,10);
			sprintf((char *)q,"%u",temp);
			q+=10;
			p+=11;
			
		}
	}
	
	//////////gsm列表/////////////////
	p=(unsigned char *)(web_set_data->gsm_list1);
	q=(unsigned char *)(center_struct.gsm_list1);
	memset(q,0,sizeof(center_struct.gsm_list1)*10);
	for(i=0;i<10;i++)
	{
		if(p[0]==0)
		{
			break;
		}
		else
		{
			memcpy(q,p,WEB_STRING_MAX_LENGTH);
			p+=(WEB_STRING_MAX_LENGTH+1);
			q+=WEB_STRING_MAX_LENGTH;
		}
	}
	//////////中转台列表/////////////////
	if(engineer_mode==ENGINEER_MODE)
	{
		temp=inet_addr( web_set_data->radio_ip);
		temp=ntohl(temp);
		center_struct.radio_ip=temp;
	
		center_struct.busi_timeslot1=web_set_data->busi_timeslot1;
		center_struct.busi_timeslot2=web_set_data->busi_timeslot2;
		center_struct.ctr_timeslot1=web_set_data->ctr_timeslot1;
		center_struct.ctr_timeslot2=web_set_data->ctr_timeslot2;
		center_struct.vol_timeslot1=web_set_data->vol_timeslot1;
		center_struct.vol_timeslot2=web_set_data->vol_timeslot2;
		center_struct.simulate_call_port=web_set_data->simulate_call_port;
		center_struct.simulate_vol_port=web_set_data->simulate_vol_port;
		center_struct.radio_id=web_set_data->radio_id;
		center_struct.connect_group_number=web_set_data->connect_group_number;
		center_struct.manager_id=web_set_data->manager_id;
		center_struct.filter_switch=web_set_data->filter_switch;
		
		
		//////////黑名单开关/////////////////
		center_struct.blacklist_switch=web_set_data->blacklist_switch;
		//////////黑名单列表/////////////////
		p=(unsigned char *)(web_set_data->black_list1);
		q=(unsigned char *)(center_struct.black_list1);
		memset(q,0,sizeof(center_struct.black_list1)*10);
		for(i=0;i<10;i++)
		{
			if(p[0]==0)
			{
				break;
			}
			else
			{
				memcpy(q,p,WEB_STRING_MAX_LENGTH);
				p+=(WEB_STRING_MAX_LENGTH+1);
				q+=WEB_STRING_MAX_LENGTH;
			}
		}
		//////////白名单开关/////////////////
		center_struct.whitelist_switch=web_set_data->whitelist_switch;
		//////////白名单列表/////////////////
		p=(unsigned char *)(web_set_data->white_list1);
		q=(unsigned char *)(center_struct.white_list1);
		memset(q,0,sizeof(center_struct.white_list1)*10);
		for(i=0;i<10;i++)
		{
			if(p[0]==0)
			{
				break;
			}
			else
			{
				memcpy(q,p,WEB_STRING_MAX_LENGTH);
				p+=(WEB_STRING_MAX_LENGTH+1);
				q+=WEB_STRING_MAX_LENGTH;
			}
		}
	}
	
	center_struct.dev_call_timeout=web_set_data->dev_call_timeout;

	center_struct.alarm_count=web_set_data->alarm_count;
	
	center_struct.alarm_display=web_set_data->alarm_show_switch;
	for(i=0;i<8;i++)
	{
		center_struct.dev_alarm_icon_show_switch[i]=0xff;
		center_struct.dev_alarm_word_show_switch[i]=0xff;
	}
	if((web_set_data-> gsm_alarm_show_switch)==0)
	{
		center_struct.dev_alarm_icon_show_switch[0]=0xef;
	}
	if((web_set_data-> radio_alarm_show_switch)==0)
	{
		center_struct.dev_alarm_icon_show_switch[1]=0xfb;
	}
	/**********************************/
	center_struct.dev_freq=web_set_data->current_freq;
	center_struct.dev_power=web_set_data->dev_power;
	center_struct.neighbor_period=web_set_data->neighbor_period/2;
	if(engineer_mode==ENGINEER_MODE)
	{	
		center_struct.locking_time=web_set_data->locking_time;
	}
	center_struct.half_variance_threshold=web_set_data->half_variance_threshold;
	center_struct.neighbor_switch=web_set_data->neighbor_switch;
	/***********************************/
	
	lseek(fd,0,SEEK_SET);
	if(sizeof(center_struct)!=write(fd,(unsigned char*)&center_struct,sizeof(center_struct)))
	{
		system("echo write Configfile fail > /tmp/web_error.txt"); 
		close(fd);
		return -1;
	}
	close(fd);
	/////////////////////修改网管共享内存，写配置文件/////////////////////
	sem_cfg_p();
	memcpy(shm_cfg_addr->local_ip.buf,web_set_data->dev_ip,sizeof(web_set_data->dev_ip));
	memcpy(shm_cfg_addr->subnet_mask.buf,web_set_data->subnet_mask,sizeof(web_set_data->subnet_mask));
	memcpy(shm_cfg_addr->gateway.buf,web_set_data->def_route,sizeof(web_set_data->def_route));
	
	shm_cfg_addr->dev_id.val=web_set_data->dev_id-1;
	shm_cfg_addr->power.val=web_set_data->dev_power;
	shm_cfg_addr->cc.val=web_set_data->dev_cc;
	shm_cfg_addr->terminal_cc.val=web_set_data->terminal_cc;
	shm_cfg_addr->stop_tans.val=web_set_data->stop_transmit;
	shm_cfg_addr->boot_mode.val=web_set_data->boot_mode_switch;
	shm_cfg_addr->reboot_strategy.val=web_set_data->reboot_strategy_switch;
	//////////////////工程模式//////////////////////
	if(engineer_mode==ENGINEER_MODE)
	{
		shm_cfg_addr->locking_time.val=web_set_data->locking_time;
		shm_cfg_addr->work_time_out.val=web_set_data->dev_busi_time_out*1000;
		shm_cfg_addr->gps_time_out.val=web_set_data->gps_time_out*1000;
		shm_cfg_addr->stun_time_out.val=web_set_data->gps_time_out*1000;
		shm_cfg_addr->start_time_out.val=web_set_data->gps_time_out*1000;
		
		shm_cfg_addr->protocol_mode.val=web_set_data->work_mode_2;
		shm_cfg_addr->moto_mode_switch.val=web_set_data->terminal_compatible;
		
		shm_cfg_addr->tempratue_alarm_start_threshold.val=web_set_data->tempratue_alarm_start_threshold;
		shm_cfg_addr->tempratue_alarm_close_threshold.val=web_set_data->tempratue_alarm_close_threshold;
		
		shm_cfg_addr->close_transmit_threshold.val=abs(web_set_data->close_transmit_rssi_threshold);
		shm_cfg_addr->resume_transmit_threshold.val=abs(web_set_data->resume_transmit_rssi_threshold);
	}
	
	//////////////////工程模式//////////////////////

	
	shm_cfg_addr->half_variance_threshold.val=web_set_data->half_variance_threshold;
	shm_cfg_addr->freq.val=web_set_data->current_freq;
	shm_cfg_addr->dev_call_timeout.val=web_set_data->dev_call_timeout;
	shm_cfg_addr->start_neighbor.val=web_set_data->neighbor_switch;
	shm_cfg_addr->neighbor_period.val=web_set_data->neighbor_period/2;
	shm_cfg_addr->neighbor_report_ai.val=web_set_data->neighbor_ai_switch;
	shm_cfg_addr->alarm_switch_status.val=web_set_data->alarm_ai_switch;
	

	sem_cfg_v();
	if(save_ini_file())
	{
		system("echo save_ini_file fail > /tmp/web_error.txt"); 
		return -1;
	}
	return 0;
}



/**
 * @md5
 * @升级文件解密
 */
void md5()
{
	a=A,b=B,c=C,d=D;

	/**//* Round 1 */
	FF (a, b, c, d, x[ 0], 7, 0xd76aa478); /**//* 1 */
	FF (d, a, b, c, x[ 1], 12, 0xe8c7b756); /**//* 2 */
	FF (c, d, a, b, x[ 2], 17, 0x242070db); /**//* 3 */
	FF (b, c, d, a, x[ 3], 22, 0xc1bdceee); /**//* 4 */
	FF (a, b, c, d, x[ 4], 7, 0xf57c0faf); /**//* 5 */
	FF (d, a, b, c, x[ 5], 12, 0x4787c62a); /**//* 6 */
	FF (c, d, a, b, x[ 6], 17, 0xa8304613); /**//* 7 */
	FF (b, c, d, a, x[ 7], 22, 0xfd469501); /**//* 8 */
	FF (a, b, c, d, x[ 8], 7, 0x698098d8); /**//* 9 */
	FF (d, a, b, c, x[ 9], 12, 0x8b44f7af); /**//* 10 */
	FF (c, d, a, b, x[10], 17, 0xffff5bb1); /**//* 11 */
	FF (b, c, d, a, x[11], 22, 0x895cd7be); /**//* 12 */
	FF (a, b, c, d, x[12], 7, 0x6b901122); /**//* 13 */
	FF (d, a, b, c, x[13], 12, 0xfd987193); /**//* 14 */
	FF (c, d, a, b, x[14], 17, 0xa679438e); /**//* 15 */
	FF (b, c, d, a, x[15], 22, 0x49b40821); /**//* 16 */

	/**//* Round 2 */
	GG (a, b, c, d, x[ 1], 5, 0xf61e2562); /**//* 17 */
	GG (d, a, b, c, x[ 6], 9, 0xc040b340); /**//* 18 */
	GG (c, d, a, b, x[11], 14, 0x265e5a51); /**//* 19 */
	GG (b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /**//* 20 */
	GG (a, b, c, d, x[ 5], 5, 0xd62f105d); /**//* 21 */
	GG (d, a, b, c, x[10], 9, 0x02441453); /**//* 22 */
	GG (c, d, a, b, x[15], 14, 0xd8a1e681); /**//* 23 */
	GG (b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /**//* 24 */
	GG (a, b, c, d, x[ 9], 5, 0x21e1cde6); /**//* 25 */
	GG (d, a, b, c, x[14], 9, 0xc33707d6); /**//* 26 */
	GG (c, d, a, b, x[ 3], 14, 0xf4d50d87); /**//* 27 */
	GG (b, c, d, a, x[ 8], 20, 0x455a14ed); /**//* 28 */
	GG (a, b, c, d, x[13], 5, 0xa9e3e905); /**//* 29 */
	GG (d, a, b, c, x[ 2], 9, 0xfcefa3f8); /**//* 30 */
	GG (c, d, a, b, x[ 7], 14, 0x676f02d9); /**//* 31 */
	GG (b, c, d, a, x[12], 20, 0x8d2a4c8a); /**//* 32 */

	/**//* Round 3 */
	HH (a, b, c, d, x[ 5], 4, 0xfffa3942); /**//* 33 */
	HH (d, a, b, c, x[ 8], 11, 0x8771f681); /**//* 34 */
	HH (c, d, a, b, x[11], 16, 0x6d9d6122); /**//* 35 */
	HH (b, c, d, a, x[14], 23, 0xfde5380c); /**//* 36 */
	HH (a, b, c, d, x[ 1], 4, 0xa4beea44); /**//* 37 */
	HH (d, a, b, c, x[ 4], 11, 0x4bdecfa9); /**//* 38 */
	HH (c, d, a, b, x[ 7], 16, 0xf6bb4b60); /**//* 39 */
	HH (b, c, d, a, x[10], 23, 0xbebfbc70); /**//* 40 */
	HH (a, b, c, d, x[13], 4, 0x289b7ec6); /**//* 41 */
	HH (d, a, b, c, x[ 0], 11, 0xeaa127fa); /**//* 42 */
	HH (c, d, a, b, x[ 3], 16, 0xd4ef3085); /**//* 43 */
	HH (b, c, d, a, x[ 6], 23, 0x04881d05); /**//* 44 */
	HH (a, b, c, d, x[ 9], 4, 0xd9d4d039); /**//* 45 */
	HH (d, a, b, c, x[12], 11, 0xe6db99e5); /**//* 46 */
	HH (c, d, a, b, x[15], 16, 0x1fa27cf8); /**//* 47 */
	HH (b, c, d, a, x[ 2], 23, 0xc4ac5665); /**//* 48 */

	/**//* Round 4 */
	II (a, b, c, d, x[ 0], 6, 0xf4292244); /**//* 49 */
	II (d, a, b, c, x[ 7], 10, 0x432aff97); /**//* 50 */
	II (c, d, a, b, x[14], 15, 0xab9423a7); /**//* 51 */
	II (b, c, d, a, x[ 5], 21, 0xfc93a039); /**//* 52 */
	II (a, b, c, d, x[12], 6, 0x655b59c3); /**//* 53 */
	II (d, a, b, c, x[ 3], 10, 0x8f0ccc92); /**//* 54 */
	II (c, d, a, b, x[10], 15, 0xffeff47d); /**//* 55 */
	II (b, c, d, a, x[ 1], 21, 0x85845dd1); /**//* 56 */
	II (a, b, c, d, x[ 8], 6, 0x6fa87e4f); /**//* 57 */
	II (d, a, b, c, x[15], 10, 0xfe2ce6e0); /**//* 58 */
	II (c, d, a, b, x[ 6], 15, 0xa3014314); /**//* 59 */
	II (b, c, d, a, x[13], 21, 0x4e0811a1); /**//* 60 */
	II (a, b, c, d, x[ 4], 6, 0xf7537e82); /**//* 61 */
	II (d, a, b, c, x[11], 10, 0xbd3af235); /**//* 62 */
	II (c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /**//* 63 */
	II (b, c, d, a, x[ 9], 21, 0xeb86d391); /**//* 64 */

	A += a;
	B += b;
	C += c;
	D += d;
}


/**
 * @CalcFileMD5
 * @升级文件解密
 */
int CalcFileMD5(char *filename, char *md5_sum)
{
	int i;
	if (filename[0]==34) 
	{
		filename[strlen(filename)-1]=0;
		strcpy(filename,filename+1);
	}
	
	if (!strcmp(filename,"exit")) 
	{
		exit(0);
	}
	
	if (!(fp=fopen(filename,"rb"))) //
	{
		system("echo CalcFileMD5 Can not open this file > /tmp/web_error.txt"); 
		return 0;
	}

	fseek(fp, 0, SEEK_END);

	if((len=ftell(fp))==-1)
	{
		system("echo CalcFileMD5 Can not calculate files which larger than 2 GB > /tmp/web_error.txt"); 
		fclose(fp);
		return 0;
	}

	rewind(fp);

	A=0x67452301,B=0xefcdab89,C=0x98badcfe,D=0x10325476;

	flen[1]=len/0x20000000;

	flen[0]=(len%0x20000000)*8;

	memset(x,0,64);

	fread(&x,4,16,fp);

	for(i=0; i<len/64; i++)
	{
		md5();
		memset(x,0,64);
		fread(&x,4,16,fp);
	}

	((char*)x)[len%64]=128;

	if(len%64>55) 
	{
		md5();
		memset(x,0,64);
	}

	memcpy(x+14,flen,8);

	md5();

	fclose(fp);

	sprintf(md5_sum,"%08x%08x%08x%08x",PP(A),PP(B),PP(C),PP(D));

	return 1;
}

/**
 * @bin_to_bz2    bin文件转化为压缩文件
 *
 * @param[in]		bin_name	bin文件名
 * @param[out]		bz2_name	压缩文件名
 * @return		    函数执行状态
 * @author		wdz
 * @bug
 */
int bin_to_bz2(char * bin_name, char * bz2_name)
{
	FILE * bin_file;
	FILE * bz2_file;
	unsigned int bin_name_len = 0;
	unsigned int bin_file_len;
    unsigned int suffixation_len = strlen(".bin");
	unsigned int i_loop = 0;
	unsigned int loop_cnt;
	int remainder;
	char buf[BUFFERSIZE];

	bin_name_len = strlen(bin_name);
	if (50 < bin_name_len)
	{
		system("echo bin_to_bz2 name too long! > /tmp/web_error.txt"); 
		return -1;
	}

	if (0 != strcmp(".bin", bin_name + bin_name_len - suffixation_len))
	{
		system("echo bin_to_bz2 err file! > /tmp/web_error.txt");
		return -1;
	}
	else
	{
		strncpy(bz2_name, bin_name, bin_name_len - suffixation_len);
		strcat(bz2_name, ".tar.bz2");
		//printf("%s\n", bz2_name);
	    bz2_file = fopen(bz2_name, "wb+");
		if (NULL == bz2_file)
		{
			system("echo bz2_file fopen err > /tmp/web_error.txt");
			return -1;
		}
		bin_file = fopen(bin_name, "rb");
		if (NULL == bin_file)
		{
			system("echo bz2_file fopen err > /tmp/web_error.txt");
			return -1;
		}

		if (MD5_LEN != (int)fread(md5_sum_in_bin, sizeof(char), MD5_LEN, bin_file))
		{
			system("echo bz2_file fread md5 err > /tmp/web_error.txt");
			fclose(bz2_file);
			fclose(bin_file);
			return -1;
		}

		fseek(bin_file, 0, SEEK_END);
		bin_file_len = ftell(bin_file);
		fseek(bin_file, FILE_INFO_LEN, SEEK_SET);
        loop_cnt = (bin_file_len - FILE_INFO_LEN) / BUFFERSIZE;
		remainder = (bin_file_len - FILE_INFO_LEN) % BUFFERSIZE;
		
		for (i_loop = 0; i_loop < loop_cnt; i_loop++)
		{
			if (BUFFERSIZE != fread(buf, sizeof(char), BUFFERSIZE, bin_file))
			{
				system("echo bz2_file fread err > /tmp/web_error.txt");
				fclose(bz2_file);
				fclose(bin_file);
				return -1;
			}
			else 
			{
				if (BUFFERSIZE != fwrite(buf, sizeof(char), BUFFERSIZE, bz2_file))
				{
					system("echo bz2_file fwrite err > /tmp/web_error.txt");
					fclose(bz2_file);
					fclose(bin_file);
					return -1;
				}
			}
		}

		if (remainder != (int)fread(buf, sizeof(char), remainder, bin_file))
		{
			system("echo bz2_file fread remainder err > /tmp/web_error.txt");
			fclose(bz2_file);
			fclose(bin_file);
			return -1;
		}
		else 
		{
			if (remainder != (int)fwrite(buf, sizeof(char), remainder, bz2_file))
			{
				system("echo bz2_file fwrite remainder err > /tmp/web_error.txt");
				fclose(bz2_file);
				fclose(bin_file);
				return -1;
			}
		}

		fclose(bz2_file);
		fclose(bin_file);
		
		if(!CalcFileMD5(bz2_name, md5_sum))
		{
			system("echo call CalcFileMD5 err > /tmp/web_error.txt");
			return -1;
		}

		if (0 != strncmp(md5_sum, md5_sum_in_bin, MD5_LEN))
		{
			system("echo invalid update file > /tmp/web_error.txt");
			return -1;
		}
    }

	return 0;
}

/**
 * @test_code    system执行结果检查
 *
 * @param[in]		status	system执行结果
 * @return		    system执行检查结果
 * @author		wdz
 * @bug
 */
int test_code(int status)
{
    if (-1 == status) 
    { 
        system("echo system return failed! > /tmp/web_error.txt");
		return -1;
			
	}
	else
    {
        if (WIFEXITED(status))
		{
		    if(0 == WEXITSTATUS(status))
			{
		        return 0;
			}
			else
			{
				system("echo WEXITSTATUS(status) failed! > /tmp/web_error.txt");   
		   		return -1;
			}
		}
		else  
		{  
			system("echo WIFEXITED(status) failed! > /tmp/web_error.txt");  
			return -1;
		   	
		}
	}
}


/**
 * @transmit_file    接收web传输升级文件
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int transmit_file()
{
	char name[128]={0};
	char fileNameOnServer[50]={0};
	char bz2_file_name[50] = {0};
	unsigned char code[200]={0};
	char buffer[1024];
	int got;
	int index;
	int targetFile;
	int status=0;
	char *tmpStr=NULL;
	cgiFilePtr file;
	
	////////////////////获取文件名/////////////////////////
	if (cgiFormFileName("file", name, sizeof(name)) != cgiFormSuccess) 
	{
		system("echo No file was uploaded > /tmp/web_error.txt"); 
		return -1;
	}
	////////////////////打开文件/////////////////////////
	if (cgiFormFileOpen("file", &file) != cgiFormSuccess) {   
		system("echo could not open the file > /tmp/web_error.txt");   
		cgiFormFileClose(file);
		return -1;    
	}  
	////////////////////根据文件名创建新文件/////////////////////////
	index=-1;    
    //从路径名解析出用户文件名    
    while(1)
	{    
		tmpStr=strstr(name+index+1,"\\");    
		if(NULL==tmpStr)			
			tmpStr=strstr(name+index+1,"/");//if "\\" is not path separator, try "/"    
		if(NULL!=tmpStr)    
			index=(int)(tmpStr-name);    
		else 
			break;    
	}    
	strcpy(fileNameOnServer,name+index+1);

    if(!strcmp(fileNameOnServer,UPDATE_LOADAPP))
	{
		chdir("/home");
	    status=system("rm -rf /home/*");
	    if(test_code(status))
	    {
			system("echo  rm -rf /home/* failed! > /tmp/web_error.txt");   
			cgiFormFileClose(file);
			return -1;	
		}
		targetFile=open(fileNameOnServer,O_RDWR|O_CREAT|O_TRUNC);
		
		if(targetFile<0)
		{
			system("echo could not open the file > /tmp/web_error.txt");  
			cgiFormFileClose(file);		
			return -1; 
		}

		while (cgiFormFileRead(file, buffer, sizeof(buffer), &got) == cgiFormSuccess)
		{
			if(got>0)
			{
				write(targetFile,buffer,got);  
			}				
		}
		cgiFormFileClose(file);
		close(targetFile);
		memset(bz2_file_name,0,sizeof(bz2_file_name));
		if (0 != bin_to_bz2(fileNameOnServer, bz2_file_name))
		{
			system("echo bin_to_bz2 failed! > /tmp/web_error.txt"); 
			system("rm -rf /home/loadapp*");
			return -1;	
		}
	
		memset(code, 0, sizeof(code));
		sprintf((char *)code, "tar -jxf %s", bz2_file_name);
		status = system((const char *)code);
		if(test_code(status))
		{
			system("echo system tar return failed! > /tmp/web_error.txt"); 
			system("rm -rf /home/loadapp*");
			return -1;	
		}
		system("rm -rf loadapp*"); 
	}
	else 
	{
		chdir("/home");
		status=system("rm -rf /home/*");
	    if(test_code(status))
	    {
			system("echo rm -rf /home/*! > /tmp/web_error.txt");   
			return -1;	
		}

		targetFile=open(fileNameOnServer,O_RDWR|O_CREAT|O_TRUNC);
		
		if(targetFile<0)
		{
			system("echo could not open the file > /tmp/web_error.txt"); 
			cgiFormFileClose(file);		
			return -1; 
		}

		while (cgiFormFileRead(file, buffer, sizeof(buffer), &got) == cgiFormSuccess)
		{
			if(got>0)
			{
				write(targetFile,buffer,got);  
			}				
		}
		cgiFormFileClose(file);
		close(targetFile);
		memset(bz2_file_name,0,sizeof(bz2_file_name));
		if (0 != bin_to_bz2(fileNameOnServer, bz2_file_name))
		{
		    system("echo bin_to_bz2 failed! > /tmp/web_error.txt"); 
		    return -1;	
		}
		memset(code, 0, sizeof(code));
		sprintf((char *)code, "tar -jxf %s", bz2_file_name);
		status = system((const char *)code);
		if(test_code(status))
		{
			system("echo system tar return failed! > /tmp/web_error.txt"); 
		    return -1;	
		}
	}
    return 0;
}


/**
 * @update_file_system    更新文件系统
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int update_file_system()
{
    int status;
    chdir("/home");
    
    status =system("/usr/sbin/flash_eraseall /dev/mtd4 > /tmp/erase.txt");
	
    if(test_code(status))
    {
		system("echo flash_eraseall /dev/mtd4 failed! > /tmp/web_error.txt");
        return -1;   
    }
    else
    {
        status =system("/usr/sbin/flashcp ramdisk.image.gz /dev/mtd4  > /tmp/erase.txt ");
        if(test_code(status))
        {
			system("echo flashcp failed! > /tmp/web_error.txt");
            return -1;
        }
    }
	
    return 0;
}


/**
 * @update_zimage    更新内核
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int update_zimage()
{
    int status;
    chdir("/home");
    
    status =system("/usr/sbin/flash_eraseall /dev/mtd3 > /tmp/erase.txt");
    if(test_code(status))
    {
        system("echo flash_eraseall /dev/mtd3 fail! > /tmp/web_error.txt");
        return -1;   
    }
    else
    {
        status =system("/usr/sbin/flashcp zImage /dev/mtd3 > /tmp/erase.txt");
        if(test_code(status))
        {
            system("echo flashcp zImage /dev/mtd3 fail! > /tmp/web_error.txt");
            return -1;
        }
    }
    return 0;
}

/**
 * @update_dtb    更新设备树
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int update_dtb()
{
    int status;
    chdir("/home");
    
    status =system("/usr/sbin/flash_eraseall /dev/mtd2 > /tmp/erase.txt");
    if(test_code(status))
	{
		system("echo flash_eraseall /dev/mtd2! > /tmp/web_error.txt"); 
		return -1;   
	}
	else
	{
		status =system("/usr/sbin/flashcp soc_system.dtb /dev/mtd2 > /tmp/erase.txt");
		if(test_code(status))
		{
			system("echo flashcp soc_system.dtb /dev/mtd2! > /tmp/web_error.txt");
		   	return -1;
		}
	}
    return 0;
}

/**
 * @update_uboot    更新boot
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int update_uboot()
{
    int status;
    chdir("/home");
    
    status =system("/usr/sbin/flash_eraseall /dev/mtd0 > /tmp/erase.txt");
    if(test_code(status))
    {
        system("echo flash_eraseall /dev/mtd0! > /tmp/web_error.txt"); 
        return -1;   
    }
    else
    {
        status =system("/usr/sbin/flashcp bootloader /dev/mtd0 > /tmp/erase.txt");
        if(test_code(status))
        {
            system("echo flashcp bootloader /dev/mtd0! > /tmp/web_error.txt");
            return -1;
        }
    }
    return 0;
}

/**
 * @confirm_mount_point    确认当前挂载分区，确定升级分区
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int  confirm_mount_point()
{
    pid_t status;

    FILE *fd;

    fd = popen("/loadapp/mount.sh", "r");

    if(fd==NULL)
    {
		system("echo open /loadapp/mount.sh file error! > /tmp/web_error.txt");
        return -1;
    }

    memset(buffer,0x00,sizeof(buffer));

    if(fgets((char*)buffer, sizeof(buffer), fd)==NULL)
    {
		system("echo get mount point error! > /tmp/web_error.txt");
        pclose(fd);
        return -1 ;
    }

    pclose(fd);

    if(strcmp((char*)buffer,"/dev/mtdblock6\n")==0)
    {
        status = system("mount -t jffs2 /dev/mtdblock7 /mnt");
    }
    else
    {
        status = system("mount -t jffs2 /dev/mtdblock6 /mnt");
    }

    if (-1 == status)
    {
		system("echo mount /dev/mtdblockx on /mnt failed! > /tmp/web_error.txt");
        return -1 ;
    }
    else
    {
        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                	chdir("/mnt");
	                return 0;
            }
            else
            {
				system("echo mount /dev/mtdblockx on /mnt WEXITSTATUS(status) failed! > /tmp/web_error.txt");
                return -1 ;
            }
        }
        else
        {
			system("echo mount /dev/mtdblockx on /mnt WIFEXITED(status) failed! > /tmp/web_error.txt");
            return -1 ;
        }
    }

}

/**
 * @update_loadapp    升级应用程序
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int update_loadapp()
{
	char buffer_active[]="active\n";
	char buffer_back[]="back\n";
	FILE * fd;
	unsigned  int size;
	int status=0;
	if(confirm_mount_point())
	{
		system("echo confirm_mount_point failed! > /tmp/web_error.txt");
		return -1;	   
	}
	status=system("rm -rf /mnt/*");
	if(test_code(status))
	{
		system("echo rm -rf /mnt/*  failed! > /tmp/web_error.txt");   
		return -1;	
	}
	status =system(" cp -rf /home/* /mnt");
	if(test_code(status))
	{
		system("echo system cp -rf * /mnt failed! > /tmp/web_error.txt");
        chdir("/");
		system("umount /mnt");
		return -1;	
	}
	
	fd=fopen("/mnt/loadflag","wb");
		       
	if(fd == NULL)
	{
		system("echo open /mnt/loadflag file error! > /tmp/web_error.txt");
		chdir("/");
		system("umount /mnt");
		return -1;
		        
	}
	size=strlen(buffer_active);
	fwrite(buffer_active,size,1,fd);
	fclose(fd);
	if(strcmp((char *)buffer,"/dev/mtdblock5\n")!=0)
	{
		               
		fd=fopen("/loadapp/loadflag","wb");
		       
		if(fd == NULL)
		{
			system("echo open /loadapp/loadflag file failed! > /tmp/web_error.txt");
		    chdir("/");
		    system("umount /mnt");
		    return -1;
		}
		size=strlen(buffer_back);
		fwrite(buffer_back,size,1,fd);
		fclose(fd); 
	}
    system("cp -rf /loadapp/nas_config.ini /mnt");
    system("cp -rf /loadapp/ConfigFile     /mnt");
    //system("cp -rf /loadapp/config.ini /mnt");
    chdir("/");
	system("umount /mnt");
	return 0;  
}


/**
 * @update_rbf    升级rbf
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */

int update_rbf()
{
    int status;
    chdir("/home");
    
    status =system("/usr/sbin/flash_eraseall /dev/mtd1 > /tmp/erase.txt");
    if(test_code(status))
    {
		system("echo flash_eraseall /dev/mtd1! > /tmp/web_error.txt");
        return -1;   
    }
    else
    {
        status =system("/usr/sbin/flashcp soc_system.rbf /dev/mtd1 > /tmp/erase.txt");
        if(test_code(status))
        {
            system("echo flashcp soc_system.rbf /dev/mtd1! > /tmp/web_error.txt");
            return -1;
        }
    }
    return 0;
}


/**
 * @shm_ipc_attch    打开IPC共享内存
 * @return		    函数执行结果
 * @author		wdz
 * @bug
 */
int shm_ipc_attch(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_IPC);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_IPC);  

	shmid_ipc = shmget(shmkey, 0, 0); //获取共享内存 
	if (-1 == shmid_ipc)  
	{
		system("echo attch shmid_ipc fail! > /tmp/web_error.txt"); 
		return -1;
	}

	shm_ipc_addr = (SHM_IPC_STRU *)shmat(shmid_ipc, NULL, 0);
	if (-1 == (int)(shm_ipc_addr))
	{
		system("echo shmat shm_ipc_addr fail! > /tmp/web_error.txt");
		return -1;
	}

	semid_ipc = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_ipc) 
	{
		system("echo semget semid_ipc fail! > /tmp/web_error.txt");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_ipc, 0, SETVAL, sem_arg)) 
    {
		system("echo semctl semid_ipc fail! > /tmp/web_error.txt");
		return -1;
	}
	return 0;
}

/**
 * @sem_ipc_p    获取IPC信号量
 * @author		wdz
 * @bug
 */
void sem_ipc_p(void)
{
	struct sembuf s_p;
	s_p.sem_num = 0;
	s_p.sem_op = -1;
	s_p.sem_flg = SEM_UNDO;
	if (-1 == semop(semid_ipc, &s_p, 1))
	{
		system("echo sem_ipc_p operation fail! > /tmp/web_error.txt");
	}
}

/**
 * @sem_ipc_v    释放IPC信号量
 * @author		wdz
 * @bug
 */ 
void sem_ipc_v(void)
{
	struct sembuf s_v;
	s_v.sem_num = 0;
	s_v.sem_op = 1;
	s_v.sem_flg = SEM_UNDO;
	if(-1 == semop(semid_ipc, &s_v, 1))
	{
		system("echo sem_ipc_v operation fail! > /tmp/web_error.txt");
	}
}

/**
 * @cgiMain
 * @入口函数
 */
int cgiMain(){
	
	WEB_NM_DATA_ITEM     web_nm_data;
	int operate_flag=0;  
    int current_div=1;//默认显示div1
	int log_flag=0;
	int update_file_name_length=0;
	int length;
	int  freq_channel=0;
	int language_switch=0;
	int engineer_mode_flag=NORMAL_MODE;
    char web_code[50]={0};
    char web_user[50]={0};
	char update_file_name[50]={0};
	memset((unsigned char *)&web_nm_data,0,sizeof(WEB_NM_DATA_ITEM));
	cgiHeaderContentType("text/html\n\n");
	
	if(strcmp(cgiRequestMethod,"GET")== 0)          //输入网址回车分支，显示登录界面
	{
		show_login(web_user,web_code,log_flag,language_switch);	
	}
	else
	{
		
		if((cgiFormSubmitClicked("reboot_operate") == cgiFormSuccess))  //重启操作分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())                      //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)  ///////////表明其它操作正在进行////////////
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=REBOOT_FLAG;  /////////////重启操作标志//////////
            sem_ipc_v();

			
			if (mgrh_msg_queue_create())               //打开mgr_local_handle创建的消息队列，接收重启指令
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->update_dtb_result ==UPDATE_FAIL)      //重启之前，判断dtb是否升级失败
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_DTB_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->update_boot_result ==UPDATE_FAIL)     //重启之前，判断boot是否升级失败
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_BOOT_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->update_ram_result ==UPDATE_FAIL)     //重启之前，判断文件系统是否升级失败
			{ 
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_RAM_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->update_rbf_result ==UPDATE_FAIL)   //重启之前，判断rbf是否升级失败（可以去掉）
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_RBF_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->update_zimage_result ==UPDATE_FAIL)   //重启之前，判断内核是否升级失败
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_ZIMAGE_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			sem_ipc_v();
			shmdt(shm_ipc_addr);
			operate_flag=REBOOT_SUCCESS_FLAG;
			send_cmd_to_queque(CMD_CODE_REBOOT);               //发送重启指令
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("get_operate") == cgiFormSuccess))   //读取操作分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())                                  //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)  ///////////表明其它操作正在进行////////////
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=READ_FLAG;  /////////////读取操作标志//////////
            sem_ipc_v();

			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=GET_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(get_data_from_file())                             //从配置文件获取数据
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=GET_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			
			if(engineer_mode_flag == NORMAL_MODE )                //用户模式，保留用户管理界面信息
			{
				///////////////////获取用户管理界面信息///////
				cgiFormStringSpaceNeeded("old_user",&length);
				cgiFormString("old_user",( char *)(web_data.old_user),length); 
		 
				cgiFormStringSpaceNeeded("new_user",&length);
				cgiFormString("new_user",(char *)(web_data.new_user),length);
		 
				cgiFormStringSpaceNeeded("old_code",&length);
				cgiFormString("old_code",(char *)(web_data.old_code),length); 
		 
				cgiFormStringSpaceNeeded("new_code",&length);
				cgiFormString("new_code",( char *)(web_data.new_code),length);
		 
				cgiFormStringSpaceNeeded("confirm_code",&length);
				cgiFormString("confirm_code",( char *)(web_data.confirm_code),length);
				
				////////////////////获取保存的用户名和密码////////////////
				cgiFormStringSpaceNeeded("web_user",&length);
				cgiFormString("web_user",(char *)(web_data.web_user),length);
				cgiFormStringSpaceNeeded("web_code",&length);
				cgiFormString("web_code",( char *)(web_data.web_code),length);
			}
			
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_data.freq_channel=0;//无效频段
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			sem_ipc_v();
			shmdt(shm_cfg_addr);
			operate_flag=GET_SUCCESS_FLAG;
			sleep(1);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_data,current_div,update_file_name,language_switch);
		}
		else if((cgiFormSubmitClicked("set_operate") == cgiFormSuccess))       //设置操作对应分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			cgiFormInteger("freq_channel",&freq_channel,0);  //获取界面保存频段标志
			if (shm_ipc_attch())                     //获取频段信息
			{ 
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				if(freq_channel !=1)         //界面保存频段标志与设备不一致，不允许配置
				{
					operate_flag=FERQ_NOT_SAME;
					web_nm_data.freq_channel=1;
					shmdt(shm_ipc_addr);
					sleep(1);
					showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
					return -1;
				}
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				if(freq_channel !=2)         //界面保存频段标志与设备不一致，不允许配置
				{
					operate_flag=FERQ_NOT_SAME;
					web_nm_data.freq_channel=2;
					shmdt(shm_ipc_addr);
					sleep(1);
					showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
					return -1;
				}
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				operate_flag=FERQ_CHANNEL_ERROR;
				web_nm_data.freq_channel=0;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)  ///////////表明其它操作正在进行////////////
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=WRITE_FLAG;  /////////////读取操作标志//////////
            sem_ipc_v();
			////////////////////配置操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=SET_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(set_data_to_file(&web_nm_data))                //配置信息保存至配置文件
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=SET_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			sem_ipc_v();
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			operate_flag=SET_SUCCESS_FLAG;
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_dtb") == cgiFormSuccess))      //升级设备树分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())   //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			
		    if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)  ///////////表明其它操作正在进行////////////
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_DTB_FLAG;  /////////////升级操作标志//////////
            sem_ipc_v();
			
			if (shm_cfg_attch())
			{
				operate_flag=UPDATE_DTB_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);
					return -1;
				}
			}
			if(transmit_file())                           //传输升级文件
			{
				operate_flag=UPDATE_DTB_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_dtb())                            //升级dtb
			{
				operate_flag=UPDATE_DTB_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				shm_ipc_addr->update_dtb_result=UPDATE_FAIL;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			shm_ipc_addr->update_dtb_result=UPDATE_SUCCESS;
			sem_ipc_v();
			operate_flag=UPDATE_DTB_SUCCESS_FALG;
			sleep(5);
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_uboot") == cgiFormSuccess))      //升级boot分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())               //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)          //判断当前有无其它操作进行
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_UBOOT_FLAG;            //升级标志置位，互斥
            sem_ipc_v();
			if (shm_cfg_attch())
			{
				operate_flag=UPDATE_UBOOT_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);
					return -1;
				}
			}
			if(transmit_file())                             //传输升级文件
			{
				operate_flag=UPDATE_UBOOT_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_uboot())                           //升级boot
			{
				operate_flag=UPDATE_UBOOT_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				shm_ipc_addr->update_boot_result=UPDATE_FAIL;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			shm_ipc_addr->update_boot_result=UPDATE_SUCCESS;
			sem_ipc_v();
			operate_flag=UPDATE_UBOOT_SUCCESS_FALG;
			sleep(5);
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_loadapp") == cgiFormSuccess))          //升级loadapp分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())                    //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)        //判断当前有无其它操作进行
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_APP_FLAG;            //置位升级标志，互斥
            sem_ipc_v();
			if (shm_cfg_attch())
			{
				operate_flag=UPDATE_APP_FAIL_FLAG;
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);
					return -1;
				}
			}
			if(transmit_file())                                        //传输升级文件
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_APP_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_loadapp())                                     //升级loadapp
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_APP_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			sem_ipc_v();
			operate_flag=UPDATE_APP_SUCCESS_FALG;
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_file") == cgiFormSuccess))        //升级文件系统分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())                      //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)       //判断当前有无其它操作进行
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_RAM_FLAG;             //置位升级标志，互斥
            sem_ipc_v();
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=UPDATE_RAM_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);
					return -1;
				}
			}
			if(transmit_file())                       //传输升级文件
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_RAM_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_file_system())           //升级文件系统
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				shm_ipc_addr->update_ram_result=UPDATE_FAIL;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_RAM_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			shm_ipc_addr->update_ram_result=UPDATE_SUCCESS;
			sem_ipc_v();
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			operate_flag=UPDATE_RAM_SUCCESS_FALG;
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_rbf") == cgiFormSuccess))         //升级rbf分支（可删除）
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())           //获取频段信息
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)           //判断当前有无其它操作进行
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_RBF_FLAG;           //置位升级标志，互斥
            sem_ipc_v();
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=UPDATE_RBF_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);
					return -1;
				}
			}
			if(transmit_file())                  //传输升级文件
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_RBF_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_rbf())                 //升级rbf
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				shm_ipc_addr->update_rbf_result=UPDATE_FAIL;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_RBF_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			shm_ipc_addr->update_rbf_result=UPDATE_SUCCESS;
			sem_ipc_v();
			operate_flag=UPDATE_RBF_SUCCESS_FALG;
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("transmit_zimage") == cgiFormSuccess)) //升级内核分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())                 //获取频段信息
			{ 
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)         //判断当前有无其它操作进行
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=UPDATE_ZIMAGE_FLAG;         //置位升级标志，互斥
            sem_ipc_v();
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=UPDATE_ZIMAGE_FAIL_FLAG ;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE )
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)!=0)||(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)!=0))
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
						shmdt(shm_ipc_addr);
						shmdt(shm_cfg_addr);
						sleep(1);
						showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
						return -1;
					}
				}
				else
				{
					system("echo modify_web_info_flag error > /tmp/web_error.txt");
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					shmdt(shm_ipc_addr);
					shmdt(shm_cfg_addr);					
					return -1;
				}
			}
			if(transmit_file())                  //传输升级文件
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_ZIMAGE_FAIL_FLAG ;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(update_zimage())             //升级内核
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				shm_ipc_addr->update_zimage_result=UPDATE_FAIL;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				shmdt(shm_cfg_addr);
				operate_flag=UPDATE_ZIMAGE_FAIL_FLAG ;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			shm_ipc_addr->update_zimage_result=UPDATE_SUCCESS;
			sem_ipc_v();
			shmdt(shm_ipc_addr);
			shmdt(shm_cfg_addr);
			operate_flag=UPDATE_ZIMAGE_SUCCESS_FALG;
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("normal_mode") == cgiFormSuccess))       //用户模式登录分支
		{
			cgiFormInteger("language_switch",&language_switch,0);     //获取语言开关
			/////开机读取配置参数
			if (shm_cfg_attch())
			{
				return -1;
			}
			if (shm_ipc_attch())
			{ 
				shmdt(shm_cfg_addr);
				return -1;
			}
			
			if(get_data_from_file())                //从配置文件中获取配置信息
			{
				shmdt(shm_cfg_addr);
				shmdt(shm_ipc_addr);
				return -1;
			}
			if(shm_ipc_addr->freq_channel == 350)           //获取频段信息
			{
				web_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_data.freq_channel=0;//无效频段
			}
			/////常规模式/////////////
			memcpy(web_data.web_user,shm_cfg_addr->web_user.buf,sizeof(web_data.web_user));
			memcpy(web_data.web_code,shm_cfg_addr->web_code.buf,sizeof(web_data.web_code));
			shm_ipc_addr->modify_web_info_flag=0;  //记录是否更改密码，为了保证安全
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_data,current_div,update_file_name,language_switch);
			/*
			memset(web_code,0,sizeof(web_code));
			memset(web_user,0,sizeof(web_user));
			cgiFormStringSpaceNeeded("web_code",&length);
			cgiFormString("web_code",web_code,length);
			cgiFormStringSpaceNeeded("web_user",&length);
			cgiFormString("web_user",web_user,length);
			
			
			if((strcmp(web_user,shm_cfg_addr->web_user.buf)!=0)||(strcmp(web_code,shm_cfg_addr->web_code.buf)!=0))
			{
				memset(web_code,0,sizeof(web_code));
				memset(web_user,0,sizeof(web_user));
				memcpy(web_code,shm_cfg_addr->web_code.buf,sizeof(web_code));
				memcpy(web_user,shm_cfg_addr->web_user.buf,sizeof(web_user));
				log_flag=ERROR_LOGIN;
				show_login(web_user,web_code,log_flag);
				shmdt(shm_cfg_addr);
				shmdt(shm_ipc_addr);
			}
			else
			{
				if(get_data_from_file())
				{
					shmdt(shm_cfg_addr);
					shmdt(shm_ipc_addr);
					return -1;
				}
				if(shm_ipc_addr->freq_channel == 350)
				{
					web_data.freq_channel=1;
				}
				else if (shm_ipc_addr->freq_channel == 410)
				{
					web_data.freq_channel=2;
				}
				else
				{
					system("echo freq_channel error > /tmp/web_error.txt"); 
					return -1;
				}
				/////常规模式/////////////
				memcpy(web_data.web_user,shm_cfg_addr->web_user.buf,sizeof(web_data.web_user));
				memcpy(web_data.web_code,shm_cfg_addr->web_code.buf,sizeof(web_data.web_code));
				shm_ipc_addr->modify_web_info_flag=0;  //记录是否更改密码，为了保证安全
				shmdt(shm_cfg_addr);
				shmdt(shm_ipc_addr);
				showHtml(engineer_mode_flag,operate_flag,&web_data,current_div,update_flag,update_file_name);
			}
			*/
			
		}
		else if((cgiFormSubmitClicked("set_code") == cgiFormSuccess))   //更改用户信息分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			
			if (shm_ipc_attch())                //获取频段信息
			{
				return -1;
			}
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			////////////////////配置操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=SET_CODE_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			memset(shm_ipc_addr->web_old_user,0,sizeof(shm_ipc_addr->web_old_user));
			memset(shm_ipc_addr->web_old_code,0,sizeof(shm_ipc_addr->web_old_code));
			memcpy(shm_ipc_addr->web_old_code, web_nm_data.web_code, sizeof(shm_ipc_addr->web_old_code));
			memcpy(shm_ipc_addr->web_old_user, web_nm_data.web_user, sizeof(shm_ipc_addr->web_old_user));
			
			memset(web_code,0,sizeof(web_code));
			memset(web_user,0,sizeof(web_user));
			cgiFormStringSpaceNeeded("new_code",&length);
			cgiFormString("new_code",web_code,length);
			cgiFormStringSpaceNeeded("new_user",&length);
			cgiFormString("new_user",web_user,length);
	
			sem_cfg_p();
			memcpy(shm_cfg_addr->web_code.buf, web_code, sizeof(shm_cfg_addr->web_code.buf));
			memcpy(shm_cfg_addr->web_user.buf, web_user, sizeof(shm_cfg_addr->web_user.buf));
			sem_cfg_v();
			shm_ipc_addr->modify_web_info_flag=1;
		//	printf("%s\n",shm_cfg_addr->web_code.buf);
			if(save_ini_file())                  
			{
				system("echo save_ini_file fail > /tmp/web_error.txt"); 
				shmdt(shm_cfg_addr);
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=SET_CODE_FAIL;
				shm_ipc_addr->modify_web_info_flag=0;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			shmdt(shm_cfg_addr);
			sem_ipc_p();
			shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
			sem_ipc_v();
			shmdt(shm_ipc_addr);
			operate_flag=SET_CODE_SUCCESS;
			sleep(1);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("engineer_pattern") == cgiFormSuccess))      //工程模式分支
		{
			cgiFormInteger("language_switch",&language_switch,0);         //获取语言开关状态
			if (shm_cfg_attch())
			{
				return -1;
			}
			
			if(get_data_from_file())                //从配置文件获取配置信息
			{
				shmdt(shm_cfg_addr);
				return -1;
			}
			if (shm_ipc_attch())
			{
				shmdt(shm_cfg_addr);
				return -1;
			}
			if(shm_ipc_addr->freq_channel == 350)          //获取频段信息
			{
				web_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_data.freq_channel=0;//无效频段
			}
			
			engineer_mode_flag=ENGINEER_MODE;
			/////工程模式/////////////
		//	memcpy(web_data.web_user,shm_cfg_addr->web_user.buf,sizeof(web_data.web_user));
		//	memcpy(web_data.web_code,shm_cfg_addr->web_code.buf,sizeof(web_data.web_code));
		//	shm_ipc_addr->modify_web_info_flag=0;  //记录是否更改密码，为了保证安全
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_data,current_div,update_file_name,language_switch);
		}
		else if((cgiFormSubmitClicked("get_operate_validate") == cgiFormSuccess))    //用户模式，读取操作之前校验分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			////////////////////获取操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				shmdt(shm_ipc_addr);
				operate_flag=GET_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE)
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)==0)&&(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)==0))
					{
						operate_flag=GET_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)==0)&&(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)==0))
					{
						operate_flag=GET_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else
				{
					shmdt(shm_cfg_addr);
					shmdt(shm_ipc_addr);
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					return -1;
				}
			}
			
			
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
		}
		else if((cgiFormSubmitClicked("set_operate_validate") == cgiFormSuccess))  //用户模式写入操作之前校验分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			cgiFormInteger("freq_channel",&freq_channel,0);  //获取界面保存频段标志
			if (shm_ipc_attch())
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				if(freq_channel !=1)         //界面保存频段标志与设备不一致，不允许配置
				{
					operate_flag=FERQ_NOT_SAME;
					web_nm_data.freq_channel=1;
					shmdt(shm_ipc_addr);
					//sleep(1);
					showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
					return -1;
				}
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				if(freq_channel !=2)         //界面保存频段标志与设备不一致，不允许配置
				{
					operate_flag=FERQ_NOT_SAME;
					web_nm_data.freq_channel=2;
					shmdt(shm_ipc_addr);
				//	sleep(1);
					showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
					return -1;
				}
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				operate_flag=FERQ_CHANNEL_ERROR;
				web_nm_data.freq_channel=0;
				shmdt(shm_ipc_addr);
			//	sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			////////////////////获取操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				shmdt(shm_ipc_addr);
				operate_flag=SET_FAIL_FLAG;
			//	sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE)
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)==0)&&(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)==0))
					{
						operate_flag=SET_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)==0)&&(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)==0))
					{
						operate_flag=SET_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else
				{
					shmdt(shm_cfg_addr);
					shmdt(shm_ipc_addr);
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					return -1;
				}
			}
			
			
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
		}
		else if((cgiFormSubmitClicked("reboot_operate_validate") == cgiFormSuccess))      //用户模式重启操作之前校验分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			////////////////////获取操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				shmdt(shm_ipc_addr);
				operate_flag=REBOOT_FAIL_FLAG;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE)
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)==0)&&(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)==0))
					{
						operate_flag=REBOOT_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)==0)&&(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)==0))
					{
						operate_flag=REBOOT_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
					}
				}
				else
				{
					shmdt(shm_cfg_addr);
					shmdt(shm_ipc_addr);
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					return -1;
				}
			}
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
		}
		else if((cgiFormSubmitClicked("set_code_validate") == cgiFormSuccess))         //用户模式修改用户信息之前校验分支
		{
			///////获取当前网页值--防止配置失败////////////////////
			engineer_mode_flag=get_init_param_from_html(&web_nm_data,&current_div,&language_switch);
			if (shm_ipc_attch())
			{
				return -1;
			} 
			if(shm_ipc_addr->freq_channel == 350)
			{
				web_nm_data.freq_channel=1;
			}
			else if (shm_ipc_addr->freq_channel == 410)
			{
				web_nm_data.freq_channel=2;
			}
			else
			{
				system("echo freq_channel error > /tmp/web_error.txt"); 
				web_nm_data.freq_channel=0;//无效频段
			}
			if(shm_ipc_addr->clear_update_status != NO_OPERATE_FLAG)
			{
				
				operate_flag=25+shm_ipc_addr->clear_update_status;
				shmdt(shm_ipc_addr);
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			sem_ipc_p();
            shm_ipc_addr->clear_update_status=USER_INFO_FLAG;
            sem_ipc_v();
			////////////////////获取操作触发//////////////
			////////////////打开配置区共享内存//////////////
			if (shm_cfg_attch())
			{
				sem_ipc_p();
				shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
				sem_ipc_v();
				shmdt(shm_ipc_addr);
				operate_flag=SET_CODE_FAIL;
				sleep(1);
				showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);
				return -1;
			}
			if(engineer_mode_flag == NORMAL_MODE)
			{
				////////////////判断当前用户是否与设备用户一致//////////////////
				if(shm_ipc_addr->modify_web_info_flag == 0)
				{
					if((strcmp(web_nm_data.web_user,shm_cfg_addr->web_user.buf)==0)&&(strcmp(web_nm_data.web_code,shm_cfg_addr->web_code.buf)==0))
					{
						operate_flag=SET_CODE_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
					}
				}
				else if(shm_ipc_addr->modify_web_info_flag == 1)
				{
					if((strcmp(web_nm_data.web_user,shm_ipc_addr->web_old_user)==0)&&(strcmp(web_nm_data.web_code,shm_ipc_addr->web_old_code)==0))
					{
						operate_flag=SET_CODE_VALIDATE_SUCCESS;
					}
					else
					{
						operate_flag=VALIDATE_FAIL;
						sem_ipc_p();
						shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
						sem_ipc_v();
					}
				}
				else
				{
					sem_ipc_p();
					shm_ipc_addr->clear_update_status=NO_OPERATE_FLAG;
					sem_ipc_v();
					system("echo modify_web_info_flag error > /tmp/web_error.txt"); 
					return -1;
				}
			}
			
			shmdt(shm_cfg_addr);
			shmdt(shm_ipc_addr);
			showHtml(engineer_mode_flag,operate_flag,&web_nm_data,current_div,update_file_name,language_switch);

		}
		else if((cgiFormSubmitClicked("submit") == cgiFormSuccess))           //登录界面提交登录信息分支
		{
			cgiFormStringSpaceNeeded("code",&length);
			cgiFormString("code",web_code,length);
			cgiFormStringSpaceNeeded("user",&length);
			cgiFormString("user",web_user,length);
			cgiFormInteger("language_switch",&language_switch,0);
			if (shm_cfg_attch())
			{
				return -1;
			}
			
			if((strcmp(web_user,shm_cfg_addr->web_user.buf)==0)&&(strcmp(web_code,shm_cfg_addr->web_code.buf)==0))
			{
				log_flag=1;
				
			}
			else if((strcmp(web_user,"admin")==0)&&(strcmp(web_code,"Hytera1993")==0))
			{
				log_flag=2;
			}
			else
			{
				log_flag=3;
			}
			shmdt(shm_cfg_addr);
			show_login(web_user,web_code,log_flag,language_switch);
		}
		

	}
	return 0;
}
