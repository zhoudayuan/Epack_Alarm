

/**
 * @alarm_tip    提示对话框
 *
 * @param[in]		btns_num		按钮数量  1：确定  2：确定 取消
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}


/**
 * @alarm_tip    提示对话框（英文版提示语较长使用）
 *
 * @param[in]		btns_num		按钮数量  1：确定  2：确定 取消
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_add(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['500px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}
/**
 * @alarm_tip_user_log    用户信息错误，返回登陆界面
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_log(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			window.location.href = "web_mgr.cgi";
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}
/**
 * @alarm_tip_update_success    升级成功，是否重启
 *
 * @param[in]		btns_num		按钮数量  1：确定  2：取消
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_update_success(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			reboot();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @RepeatWrite    重启成功，跳转页面
 *
 * @author		wdz
 * @bug
 */
function RepeatWrite(){  
    // 保存写入的内容  
	var language_switch=$("#language_switch").val();
    var newContent = "<!DOCTYPE html>";
	newContent += "<html lang=\"en\" class=\"no-js\">";
	newContent +="<head>";
	newContent +="<meta charset=\"utf-8\">";
	newContent +="<link rel=\"stylesheet\" href=\"../css/style.css\">";
	newContent +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
	newContent +="<style type=\"text/css\">";
	newContent +="body {background-image: url(../img/1.jpg); background-size: 100% 100%;background-repeat: no-repeat;}";
	newContent +=".page-container {position: absolute;top: 50%;width: 100%;margin: -250px auto 0 auto;}";
	newContent +=".tip {margin: 0 auto;padding: 20px;text-align: center;display: table;background: rgba(255,255,255,0.05);border: solid 1px rgba(255,255,255,0.3);padding-right: 20px;}";
	newContent +=".tip img {padding-right: 20px;}";
	newContent +="h1,h2,.error-bt { text-shadow: 0 1px 4px rgba(0,0,0,.2); color: #fff; text-align: left;}";
	newContent +="h1  { font-size: 40px;line-height: 60px; }";
	newContent +="h2  { font-size: 40px; }";
	newContent +=".error-bt { cursor: pointer;width: 100%;height: 44px;margin-top: 25px;background: #e0a400;border-radius: 4px;border: 0px;font-size: 20px;color: #fff;text-align: center;}";
	newContent +="</style>";
	newContent +="<script>";
	newContent +="function log(){window.location.href = \"web_mgr.cgi\";}";
	newContent +="</script>";
	newContent +="</head>"  
    newContent += "<body>";
	newContent +="<div class=\"page-container\">";
	newContent +=" <div class=\"tip\">";
	newContent +=" <div class=\"fl\"><img src=\"../img/linkerror.png\" width=\"60\" height=\"60\" alt=\"\"/></div>";
	newContent +=" <div class=\"fl\">";
	if(0==language_switch)
	{
		newContent +="<h1>设备重启中，请稍后重新连接!</h1>" ;
	}
	else
	{
		newContent +="<h1>The device restarts, Please reconnect later!</h1>" ;
	}
	newContent +="<div class=\"clear\"></div>" ;
	newContent +=" </div>";
	newContent +=" <div>";
	if(0==language_switch)
	{
		newContent +=" <button  id =\"error\" class=\"error-bt\" type=\"button\" onclick=\"log();\">重 新 加 载</button>";
	}
	else
	{
		newContent +=" <button  id =\"error\" class=\"error-bt\" type=\"button\" onclick=\"log();\">Reload</button>";
	}
	
	newContent +=" </div>";
	newContent +=" </div>";
	newContent +=" </div>";
    newContent += "</body></html>"  
    // 写入新的内容  
    document.write(newContent);  
    document.close();  
 }  

/**
 * @alarm_tip_groupNumber_return    组呼号输入无效提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_groupNumber_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			for(var j=1;j<11;j++)
																			{
																				var team_string='team';
																				team_string=team_string+j;
																				document.getElementById(team_string).readOnly=false;
																			}
																			var num= document.getElementById('invalidate_groupNumber').value;
																			var team_string='team';
																			team_string=team_string+num;
																			document.getElementById(team_string).disabled=false;
																			document.getElementById(team_string).focus();
																			
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}


/**
 * @alarm_tip_groupNumber_error    组呼号写入之前错误提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */

function alarm_tip_groupNumber_error(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_groupNumber').value;
																			var team_string='team';
																			team_string=team_string+num;
																			document.getElementById(team_string).disabled=false;
																			document.getElementById(team_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_freqNumber_return    频率输入无效提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_freqNumber_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			for(var j=1;j<11;j++)
																			{
																				var freq_string='frePoint';
																				freq_string=freq_string+j;
																				document.getElementById(freq_string).readOnly=false;
																			}
																			var num= document.getElementById('invalidate_freqNumber').value;
																			var freq_string='frePoint';
																			freq_string=freq_string+num;
																			document.getElementById(freq_string).disabled=false;
																			document.getElementById(freq_string).focus();
																			
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_freqNumber_error    频率写入之前错误提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_freqNumber_error(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_freqNumber').value;
																			var freq_string='frePoint';
																			freq_string=freq_string+num;
																			document.getElementById(freq_string).disabled=false;
																			document.getElementById(freq_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_blackNumber_return    黑名单输入无效提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_blackNumber_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			for(var j=1;j<11;j++)
																			{
																				var black_string='blkList';
																				black_string=black_string+j;
																				document.getElementById(black_string).readOnly=false;
																			}
																			var num= document.getElementById('invalidate_blackNumber').value;
																			var black_string='blkList';
																			black_string=black_string+num;
																			document.getElementById(black_string).disabled=false;
																			document.getElementById(black_string).focus();
																			
																			}},
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}
/**
 * @alarm_tip_blackNumber_error    黑名单写入之前错误提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_blackNumber_error(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_blackNumber').value;
																			var black_string='blkList';
																			black_string=black_string+num;
																			document.getElementById(black_string).disabled=false;
																			document.getElementById(black_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_whiteNumber_return    白名单输入无效提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_whiteNumber_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			for(var j=1;j<11;j++)
																			{
																				var white_string='whtList';
																				white_string=white_string+j;
																				document.getElementById(white_string).readOnly=false;
																			}
																			var num= document.getElementById('invalidate_whiteNumber').value;
																			var white_string='whtList';
																			white_string=white_string+num;
																			document.getElementById(white_string).disabled=false;
																			document.getElementById(white_string).focus();
																			
																			}},
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}
/**
 * @alarm_tip_whiteNumber_error   白名单写入之前错误提示
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_whiteNumber_error(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_whiteNumber').value;
																			var white_string='whtList';
																			white_string=white_string+num;
																			document.getElementById(white_string).disabled=false;
																			document.getElementById(white_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_gsmNumber_return    gsm输入无效提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_gsmNumber_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			for(var j=1;j<11;j++)
																			{
																				var gsm_string='gsmComm';
																				gsm_string=gsm_string+j;
																				document.getElementById(gsm_string).readOnly=false;
																			}
																			var num= document.getElementById('invalidate_gsmNumber').value;
																			var gsm_string='gsmComm';
																			gsm_string=gsm_string+num;
																			document.getElementById(gsm_string).disabled=false;
																			document.getElementById(gsm_string).focus();
																			
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_gsmNumber_error    gsm写入操作之前错误提示
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_gsmNumber_error(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_gsmNumber').value;
																			var gsm_string='gsmComm';
																			gsm_string=gsm_string+num;
																			document.getElementById(gsm_string).disabled=false;
																			document.getElementById(gsm_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_gsmNumber_error_en    gsm写入操作之前错误提示（英文）
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_gsmNumber_error_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_gsmNumber').value;
																			var gsm_string='gsmComm';
																			gsm_string=gsm_string+num;
																			document.getElementById(gsm_string).disabled=false;
																			document.getElementById(gsm_string).focus();
																			}},
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}
/**
 * @alarm_tip_save_return    写入操作提示框
 *
 * @param[in]		btns_num		按钮数量  1：确定  
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
function alarm_tip_save_return(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, yes : function(index){ layer.close(index);document.getElementById("tmp_team").value=$("#teamIdNow").val();
																						document.getElementById("tmp_freq").value=$("#frePointNow").val();
																						var language_switch=$("#language_switch").val();
																						if(document.getElementById("current_mode").value==0)
																						{
																							document.getElementById("set_operate_validate").click(); 
																						}
																						else
																						{
																							document.getElementById("set_operate").click(); 
																							
																							if(0==language_switch)
																							{
																								layer.load('参数配置中，请等待。。。。');
																							}
																							else
																							{
																								layer.load('Configuring parameters, please wait......');
																							}
																						}
																						
																					}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}

/**
 * @alarm_tip_import_file    工程模式导入常规模式文件提示框
 *
 * @param[in]		btns_num		按钮数量  2：确定 取消 
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
 
function alarm_tip_import_file(btns_num,btn1,btn2,option_info,option_title,arr){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, yes : function(index){ layer.close(index);var net_param_start=2;
																						var base_conf_start=8;
																						var team_conf_start=33;
																						var freq_conf_start=45;
																						var gsm_start=57;
																						var vertion_info_start=68;
																						var language_switch=$("#language_switch").val();
																						setNetParam(arr[net_param_start+1],arr[net_param_start+2],arr[net_param_start+3],arr[net_param_start+4],arr[net_param_start+5]);
																						setBasConf(arr[base_conf_start+1],arr[base_conf_start+2],arr[base_conf_start+3],arr[base_conf_start+4],arr[base_conf_start+5],arr[base_conf_start+6],arr[base_conf_start+7],arr[base_conf_start+8],arr[base_conf_start+9],arr[base_conf_start+10],arr[base_conf_start+11],arr[base_conf_start+12],arr[base_conf_start+13],arr[base_conf_start+14]
																								,arr[base_conf_start+15],arr[base_conf_start+16],arr[base_conf_start+17],arr[base_conf_start+18],arr[base_conf_start+19],arr[base_conf_start+20],arr[base_conf_start+21],arr[base_conf_start+22],arr[base_conf_start+23],arr[base_conf_start+24]);
																						setTeamConf(arr[team_conf_start+1],arr[team_conf_start+2],arr[team_conf_start+3],arr[team_conf_start+4],arr[team_conf_start+5],arr[team_conf_start+6],arr[team_conf_start+7],arr[team_conf_start+8],arr[team_conf_start+9],arr[team_conf_start+10],arr[team_conf_start+11]);    
																						setFreConf(arr[freq_conf_start+1],arr[freq_conf_start+2],arr[freq_conf_start+3],arr[freq_conf_start+4],arr[freq_conf_start+5],arr[freq_conf_start+6],arr[freq_conf_start+7],arr[freq_conf_start+8],arr[freq_conf_start+9],arr[freq_conf_start+10],arr[freq_conf_start+11]);
																						setGsmConf(arr[gsm_start+1],arr[gsm_start+2],arr[gsm_start+3],arr[gsm_start+4],arr[gsm_start+5],arr[gsm_start+6],arr[gsm_start+7],arr[gsm_start+8],arr[gsm_start+9],arr[gsm_start+10]);
																						//setMidCovtConf(arr[midCovtConf_start+1],arr[midCovtConf_start+2],arr[midCovtConf_start+3],arr[midCovtConf_start+4],arr[midCovtConf_start+5],arr[midCovtConf_start+6],arr[midCovtConf_start+7],arr[midCovtConf_start+8],arr[midCovtConf_start+9],arr[midCovtConf_start+10],arr[midCovtConf_start+11],arr[midCovtConf_start+12],arr[midCovtConf_start+13]);
																						setVertionConf(arr[vertion_info_start+1],arr[vertion_info_start+2]);
																						
																						if(0==language_switch)
																						{
																							alarm_tip(1,'确定','取消',"导入文件成功",'温馨提示');
																						}
																						else
																						{
																							alarm_tip(1,'OK','Cancel',"File imported successfully",'Prompt');
																						}
																						
																						}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}

/**
 * @alarm_tip_import_file_en    工程模式导入常规模式文件提示框(英文)
 *
 * @param[in]		btns_num		按钮数量  2：确定 取消 
 * @param[in]		option_info		提示信息
 * @author		wdz
 * @bug
 */
 
function alarm_tip_import_file_en(btns_num,btn1,btn2,option_info,option_title,arr){
	$.layer({
				area:['410px','160px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, yes : function(index){ layer.close(index);var net_param_start=2;
																						var base_conf_start=8;
																						var team_conf_start=33;
																						var freq_conf_start=45;
																						var gsm_start=57;
																						var vertion_info_start=68;
																						var language_switch=$("#language_switch").val();
																						setNetParam(arr[net_param_start+1],arr[net_param_start+2],arr[net_param_start+3],arr[net_param_start+4],arr[net_param_start+5]);
																						setBasConf(arr[base_conf_start+1],arr[base_conf_start+2],arr[base_conf_start+3],arr[base_conf_start+4],arr[base_conf_start+5],arr[base_conf_start+6],arr[base_conf_start+7],arr[base_conf_start+8],arr[base_conf_start+9],arr[base_conf_start+10],arr[base_conf_start+11],arr[base_conf_start+12],arr[base_conf_start+13],arr[base_conf_start+14]
																								,arr[base_conf_start+15],arr[base_conf_start+16],arr[base_conf_start+17],arr[base_conf_start+18],arr[base_conf_start+19],arr[base_conf_start+20],arr[base_conf_start+21],arr[base_conf_start+22],arr[base_conf_start+23],arr[base_conf_start+24]);
																						setTeamConf(arr[team_conf_start+1],arr[team_conf_start+2],arr[team_conf_start+3],arr[team_conf_start+4],arr[team_conf_start+5],arr[team_conf_start+6],arr[team_conf_start+7],arr[team_conf_start+8],arr[team_conf_start+9],arr[team_conf_start+10],arr[team_conf_start+11]);    
																						setFreConf(arr[freq_conf_start+1],arr[freq_conf_start+2],arr[freq_conf_start+3],arr[freq_conf_start+4],arr[freq_conf_start+5],arr[freq_conf_start+6],arr[freq_conf_start+7],arr[freq_conf_start+8],arr[freq_conf_start+9],arr[freq_conf_start+10],arr[freq_conf_start+11]);
																						setGsmConf(arr[gsm_start+1],arr[gsm_start+2],arr[gsm_start+3],arr[gsm_start+4],arr[gsm_start+5],arr[gsm_start+6],arr[gsm_start+7],arr[gsm_start+8],arr[gsm_start+9],arr[gsm_start+10]);
																						//setMidCovtConf(arr[midCovtConf_start+1],arr[midCovtConf_start+2],arr[midCovtConf_start+3],arr[midCovtConf_start+4],arr[midCovtConf_start+5],arr[midCovtConf_start+6],arr[midCovtConf_start+7],arr[midCovtConf_start+8],arr[midCovtConf_start+9],arr[midCovtConf_start+10],arr[midCovtConf_start+11],arr[midCovtConf_start+12],arr[midCovtConf_start+13]);
																						setVertionConf(arr[vertion_info_start+1],arr[vertion_info_start+2]);
																						
																						if(0==language_switch)
																						{
																							alarm_tip(1,'确定','取消',"导入文件成功",'温馨提示');
																						}
																						else
																						{
																							alarm_tip(1,'OK','Cancel',"File imported successfully",'Prompt');
																						}
																						
																						}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}
//////////////////////////加密///////////////////

/**
 * @Base64    导出文件加解密
 * @author		wdz
 */
function Base64() {
 
    // private property
    _keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
 
    // public method for encoding
    this.encode = function (input) {
        var output = "";
        var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
        var i = 0;
        input = _utf8_encode(input);
        while (i < input.length) {
            chr1 = input.charCodeAt(i++);
            chr2 = input.charCodeAt(i++);
            chr3 = input.charCodeAt(i++);
            enc1 = chr1 >> 2;
            enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
            enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
            enc4 = chr3 & 63;
            if (isNaN(chr2)) {
                enc3 = enc4 = 64;
            } else if (isNaN(chr3)) {
                enc4 = 64;
            }
            output = output +
            _keyStr.charAt(enc1) + _keyStr.charAt(enc2) +
            _keyStr.charAt(enc3) + _keyStr.charAt(enc4);
        }
        return output;
    }
 
    // public method for decoding
    this.decode = function (input) {
        var output = "";
        var chr1, chr2, chr3;
        var enc1, enc2, enc3, enc4;
        var i = 0;
        input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
        while (i < input.length) {
            enc1 = _keyStr.indexOf(input.charAt(i++));
            enc2 = _keyStr.indexOf(input.charAt(i++));
            enc3 = _keyStr.indexOf(input.charAt(i++));
            enc4 = _keyStr.indexOf(input.charAt(i++));
            chr1 = (enc1 << 2) | (enc2 >> 4);
            chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
            chr3 = ((enc3 & 3) << 6) | enc4;
            output = output + String.fromCharCode(chr1);
            if (enc3 != 64) {
                output = output + String.fromCharCode(chr2);
            }
            if (enc4 != 64) {
                output = output + String.fromCharCode(chr3);
            }
        }
        output = _utf8_decode(output);
        return output;
    }
 
    // private method for UTF-8 encoding
    _utf8_encode = function (string) {
        string = string.replace(/\r\n/g,"\n");
        var utftext = "";
        for (var n = 0; n < string.length; n++) {
            var c = string.charCodeAt(n);
            if (c < 128) {
                utftext += String.fromCharCode(c);
            } else if((c > 127) && (c < 2048)) {
                utftext += String.fromCharCode((c >> 6) | 192);
                utftext += String.fromCharCode((c & 63) | 128);
            } else {
                utftext += String.fromCharCode((c >> 12) | 224);
                utftext += String.fromCharCode(((c >> 6) & 63) | 128);
                utftext += String.fromCharCode((c & 63) | 128);
            }
 
        }
        return utftext;
    }
 
    // private method for UTF-8 decoding
    _utf8_decode = function (utftext) {
        var string = "";
        var i = 0;
        var c = c1 = c2 = 0;
        while ( i < utftext.length ) {
            c = utftext.charCodeAt(i);
            if (c < 128) {
                string += String.fromCharCode(c);
                i++;
            } else if((c > 191) && (c < 224)) {
                c2 = utftext.charCodeAt(i+1);
                string += String.fromCharCode(((c & 31) << 6) | (c2 & 63));
                i += 2;
            } else {
                c2 = utftext.charCodeAt(i+1);
                c3 = utftext.charCodeAt(i+2);
                string += String.fromCharCode(((c & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
                i += 3;
            }
        }
        return string;
    }
}

/**
 * @showDiv    显示当前div
 * @param[in]   num		当前div
 * @author		wdz
 */
function showDiv(num){
	
	var mode=document.getElementById("current_mode").value;  
	var count=9+3*parseInt(mode);
    for(var i=1;i<count;i++)
    {
		var hideId = "show"+i;
		document.getElementById(hideId).style.display="none";
		var remId = "li"+i;
		$("#"+remId).removeClass("on");
    }
    var showId = "show"+num;
    document.getElementById(showId).style.display="";
	var div_num=num +"\0";
	document.getElementById("current_div").value=div_num;
    var liId = "li"+num;
	document.getElementById(liId).className="on";
}

/**
 * @disable_back    禁用后退按钮，刷新操作不重复之前操作
 * @author		wdz
 */
function disable_back()
{
	history.pushState(null, null, document.URL);
    window.addEventListener('popstate', function () {
        history.pushState(null, null, document.URL);
        });
}

/**
 * @submit    写入操作对应函数
 * @author		wdz
 */
function submit()
{
	var language_switch=$("#language_switch").val();   //0：中文；1：英文
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
    if(freq_channel==0)
    {
	   if(language_switch==0)
	   {
		   alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		   return;
	   }
	   else
	   {
		   alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		   return;
	   }
	   
    }
	if(validate_linkIp()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机IP输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Local IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_linkIp_add()==0)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机IP不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Local IP Address cannot contain extra zeros','Prompt');
		}
		return ;
    }
	if(validate_mask()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Subnet Mask is wrong','Prompt');
		}
		return;
    }
	if(validate_mask_add()==0)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot contain extra zeros','Prompt');
		}
		return;
    }
	var  mask_return =validate_mask_value();
	if(mask_return ==1)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能为0.0.0.0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot be 0.0.0.0','Prompt');
		}
		return ;
    }
	else if(2==mask_return)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能为255.255.255.255','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot be 255.255.255.255','Prompt');
		}
		return ;
	}
	if(validate_mask_continue()==0)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码无效，子网掩码必须相邻','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Invalid subnet mask. The subnet mask has to be contiguous','Prompt');
		}
		return;
    }
	var mask_and_ip=validate_mask_and_ip();
	if(mask_and_ip==1)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与本机IP组合无效，本机IP主机地址所有位不能均为0','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and local IP address is invalid. It is disallowed that all of the bits in the host address portion of the IP address are set to 0','Prompt');
		}
		return;
    }
	else if(mask_and_ip==2)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与本机IP组合无效，本机IP主机地址所有位不能均为1','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and local IP address is invalid. It is disallowed that all of the bits in the host address portion of the IP address are set to 1','Prompt');
		}
		return;
	}
		
	if(validate_gateWay()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Gateway IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_gateWay_add()==0)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Gateway IP Address cannot contain extra zeros','Prompt');
		}
		return;
    }
	if(validate_gateWay_value()==0)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关与本机IP不在同一个网段','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Gateway IP Address and Local IP Address are in different network segments','Prompt');
		}
		return;
    }
	var mask_and_gateway=validate_mask_and_gateway();
	if(mask_and_gateway==1)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与网关组合无效，网关主机地址所有位不能均为0','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and gateway IP address is invalid. It is disallowed that all of the bits in the host address portion of the gateway IP address are set to 0','Prompt');
		}
		return;
    }
	else if(mask_and_gateway==2)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与网管组合无效，网关主机地址所有位不能均为1','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and gateway IP address is invalid. It is disallowed that all of the bits in the host address portion of the gateway IP address are set to 1','Prompt');
		}
		return;
	}
	if(validate_centerIP()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中心IP输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Center IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_centerIP_add()==0)
	{
		showDiv(1);
		document.getElementById('centerIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中心IP不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Center IP address cannot contain extra zeros','Prompt');
		}
		return ;
    }
	if(validate_centerID()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','互联ID输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Interconnection ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_epkID()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机编号输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Local Device ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_intercomID()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','对讲ID输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Radio ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_power_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','发射功率输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of TX Power is wrong','Prompt');
		}
		return ;
    }
	if(validate_epkCode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','链路色码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Link Color Code is wrong','Prompt');
		}
		return ;
    }
	if(validate_termCode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','终端色码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Radio Color Code is wrong','Prompt');
		}
		return ;
    }
	if(validate_code()== false)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','终端色码应不同于链路色码','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Link Color Code and Radio Color Code must be different','Prompt');
		}
		return ;
	}
	
	if(validate_muteThreshold_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','静噪门限输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Squelch Threshold is wrong','Prompt');
		}
		return ;
	}
	if(validate_micVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','MIC音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of MIC Volume is wrong','Prompt');
		}
		
		return ;
	}
	if(validate_gsmVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','GSM音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of GSM Volume is wrong','Prompt');
		}
		return ;
	}
	if(validate_trumpetVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','手咪默认音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Palm Microphone Volume is wrong','Prompt');
		}
		return ;
	}
	if(validate_devCallTimeout()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','通话限时输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Call Duration Limit is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_switch_status_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点查询使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Neighbor Query is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_period_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点周期输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Neighbor Query Interval is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_ai_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点上报使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Report Neighbor to Center is wrong','Prompt');
		}
		return ;
    }
	if(validate_stop_transmit()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','智能中转输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Smart Repeating is wrong','Prompt');
		}
		return ;
    }
	if(validate_dev_language_switch()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','语言开关输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Language Switch is wrong','Prompt');
		}
		return ;
    }
	if(validate_boot_mode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','开机方式输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Boot Method is wrong','Prompt');
		}
		return ;
    }
	if(validate_reboot_strategy_switch()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','启动策略输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'确定','取消','The format of Startup Policy is wrong','温馨提示');
		}
		return ;
    }
	if(validate_alarm_switch_status_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','告警上报使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Alarm Reporting is wrong','Prompt');
		}
		return ;
    }
	
	if(validate_alarm_count()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','紧急警报重复次数输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Emergency Alarm Transmissions is wrong','Prompt');
		}
		return ;
    }
	if(validate_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','紧急警报显示使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Display Emergency Alarm is wrong','Prompt');
		}
		
		return ;
    }
	if(validate_gsm_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','GSM告警使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of GSM Alarm is wrong','Prompt');
		}
		return ;
    }
	if(validate_radio_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中转台告警使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Repeater Alarm is wrong','Prompt');
		}
		
		return ;
    }
	//////////////////验证组呼号////////////////////////////
	var team_return=before_write_validate_groupNumber();
	if(team_return==0)
	{	
		return ;
	}
	if(validate_IdNow_in_team()==false)
	{
		showDiv(3);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前组不在组1~组10列表中','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The current group is out of Groups 1-10','Prompt');
		}
		return ;
	}
	var freq_return=before_write_validate_freqlist();
	if(freq_return==0)
	{
		return ;
	}
	if(validate_freqNow_in_team()==false)
	{
		showDiv(4);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前频率不在频率1~频率10列表中','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The current frequency is out of Frequencies 1-10','Prompt');
		}
		return ;
	}
	
	var gsmComm_return=before_write_validate_gsmNumber();
	if(gsmComm_return0=0)
	{
		return ;	
	}
	
	////////////////工程模式//////////////////////
	if(document.getElementById("current_mode").value==1)    //0：代表用户模式  1：代表工程模式
	{
		if(validate_blkListConf_value()==0)
		{
			showDiv(8);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','黑名单使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Blacklist is wrong','Prompt');
			}
			return ;
		}
		
		var blklist_return=before_write_validate_blackNumber();
		if(blklist_return==0)
		{
			return ;
		}
		if(validate_whtListConf_value()==0)
		{
			showDiv(8);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','白名单使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Whitelist is wrong','Prompt');
			}
			return ;
		}
		var whtList_return=before_write_validate_whiteNumber();
		if(whtList_return==0)
		{
			return ;
		}
	
		if(validate_radioIP()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台IP输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater IP Address is wrong','Prompt');
			}
			return ;
		}
		if(validate_radioIP_add()==0)
		{
			showDiv(10);
			document.getElementById('radioIP').style.borderColor="red";
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台IP不能包含多余的0','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The Repeater IP Address cannot contain extra zeros','Prompt');
			}
			return ;
		}
		if(validate_radioID()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台ID输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater ID is wrong','Prompt');
			}
			return ;
		}
		if(validate_interConctNum()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转组输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater Group is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_msgCtrlAdmID()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','短信控制管理员ID输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Message Manager ID is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_msgCtrlFilter_value()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','短信控制过滤使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Message Filter is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_gsm_call_interval()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','GSM主叫重呼间隔输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of GSM Recall Interval is wrong','Prompt');
			}
			return ;
		}
	
		if(validate_gsm_reboot_time()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','GSM模块异常重启时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of GSM Fault Recovery Interval is wrong','Prompt');
			}
			return ;
		}
		if(validate_vol_start_threshold_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测启动门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Detection Start Threshold is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_vol_delay()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测拖尾延时输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Speak Right Reset Delay is wrong','Prompt');
			}
			return ;
		}
	
		if(validate_vol_energy_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测能量阈值输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Detection Energy Threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_vol_packet_time_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音包时长输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Packet Length is wrong','Prompt');
			}
			return ;
		}
		if(validate_terminal_compatible_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','终端兼容模式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Support Motorola Radios is wrong','Prompt');
			}
			return ;
		}
		if(validate_work_mode_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','工作模式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Operation Mode is wrong','Prompt');
			}
			return ;
		}
		if(validate_voice_mode_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音编码格式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Voice Codec is wrong','Prompt');
			}
			return ;
		}
		if(validate_linkTime_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','链路保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Link Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_devOverTime()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','链路业务保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Link Service Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_termGpsOverTime()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','终端业务保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Radio Service Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_close_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','关闭转发场强门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeating Stop RSSI threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_resume_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','恢复转发场强门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeating Recover RSSI threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_tempratue_alarm_start_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','温度告警门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Temperature Alarm Threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_tempratue_alarm_close_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','温度告警恢复门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Temperature Alarm Recovery Threshold is wrong','Prompt');
			}
			return ;
		}
	}
	if(0==language_switch)
	{
		alarm_tip_save_return(2,'确定','取消','参数配置成功会自动重启，是否配置','温馨提示');
	}
	else
	{
		alarm_tip_save_return(2,'OK','Cancel','Device will restart upon successful configuration. Do you want to continue?','Prompt');
	}
}


/**
 * @readParam_validate    用户模式读取操作之前先验证用户信息
 * @author		wdz
 */
function readParam_validate()
{
	var language_switch=$("#language_switch").val();
	document.getElementById("read_down").value=1;       //  置位为1，在组呼、频率 、gsm、黑白名单输入无效情况下可以点击读取操作
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
    if(freq_channel==0)
    {
	   if(language_switch==0)
	   {
		   alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		   return;
	   }
	   else
	   {
		   alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		   return;
	   }
	   
    }
	for(var i=1;i<11;i++)
	{   
		var team_string='team';
		team_string=team_string+i;
		document.getElementById(team_string).disabled=false;
	}
	for(var i=1;i<11;i++)
	{   
		var fre_string='frePoint';
		fre_string=fre_string+i;
		document.getElementById(fre_string).disabled=false;
	}
	for(var i=1;i<11;i++)
	{   
		var gsm_string='gsmComm';
		gsm_string=gsm_string+i;
		document.getElementById(gsm_string).disabled=false;
	}
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	
	if(document.getElementById("current_mode").value==1)
	{
		for(var i=1;i<11;i++)
		{   
			var blk_string='blkList';
			blk_string=blk_string+i;
			document.getElementById(blk_string).disabled=false;
		}
		for(var i=1;i<11;i++)
		{   
			var wht_string='whtList';
			wht_string=wht_string+i;
			document.getElementById(wht_string).disabled=false;
		}
		document.getElementById("get_operate").click();
		if(0==language_switch)
		{
			layer.load('参数读取中，请等待。。。。');
		}
		else
		{
			layer.load('Reading parameters, please wait......');
		}
		
	}
	else
	{
		document.getElementById("get_operate_validate").click();
	}
}


/**
 * @readParam    读取操作
 * @author		wdz
 */
	
function readParam()
{
	var language_switch=$("#language_switch").val();
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("get_operate").click();
	if(0==language_switch)
	{
		layer.load('参数读取中，请等待。。。。');
	}
	else
	{
		layer.load('Reading parameters, please wait......');
	}
}

/**
 * @writeParam    写入操作
 * @author		wdz
 */
function writeParam()
{
	var language_switch=$("#language_switch").val();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("set_operate").click();
	if(0==language_switch)
	{
		layer.load('参数配置中，请等待。。。。');
	}
	else
	{
		layer.load('Configuring parameters, please wait......');
	}
	
}


/**
 * @reboot_validate    用户模式重启操作之前验证用户信息
 * @author		wdz
 */
function reboot_validate()
{
	var language_switch=$("#language_switch").val();
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	if(document.getElementById("current_mode").value==1)
	{
		document.getElementById("reboot_operate").click();
		if(0==language_switch)
		{
			layer.load('设备重启中，请等待。。。。');
		}
		else
		{
			layer.load('Device is restarting, please wait......');
		}
		
	}
	else
	{
		document.getElementById("reboot_operate_validate").click();
	}
}

/**
 * @reboot    重启操作
 * @author		wdz
 */

function reboot()
{
	var language_switch=$("#language_switch").val();
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("reboot_operate").click();
	if(0==language_switch)
	{
		layer.load('设备重启中，请等待。。。。');
	}
	else
	{
		layer.load('Device is restarting, please wait......');
	}
}

/**
 * @showTeam    刷新页面显示组呼列表
 * @author		wdz
 */
function  showTeam()
{
	var language_switch=$("#language_switch").val();  //当前语言开关状态     0：中文 1：英文
	var invalidate_num=0;
	for(var i=1;i<11;i++)
	{
		var team_string='team';
		team_string=team_string+i;
		
		if(validate_team_item(team_string)==0)         //查找第一个无效组呼列表，清空，enable、添加title
		{
			invalidate_num=i;
			break
		}
	}
	
	if(invalidate_num ==0)
	{
		invalidate_num=11;
	}
	
	for(var i=1;i<invalidate_num;i++)
	{
		var team_num='team';
		team_num=team_num+i;
		var base_value=document.getElementById(team_num).value;
		
		for(var j=i+1;j<invalidate_num;j++)
		{
			team_num='team';
			team_num=team_num+j;
			var current_value=document.getElementById(team_num).value;
			if(base_value==current_value)
			{
				invalidate_num=j;
				break;
			}
		}
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var team_string='team';
		team_string=team_string+i;
		document.getElementById(team_string).disabled=false;
		if(0==language_switch)
		{
			if(i==1)
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415,不能为空";
			}
			else
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415";
			}
		}
		else
		{
			if(i==1)
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415. It cannot be empty";
			}
			else
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415";
			}
		}
	}
	if(invalidate_num !=11)
	{
		var team_string='team';
		team_string=team_string+invalidate_num;
		document.getElementById(team_string).value="";
		document.getElementById(team_string).disabled=false;
		
		
		if(0==language_switch)
		{
			if(i==1)
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415,不能为空";
			}
			else
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415";
			}
		}
		else
		{
			if(i==1)
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415. It cannot be empty";
			}
			else
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415";
			}
		}
		for(var i=invalidate_num+1;i<11;i++)
		{
			var team_string='team';
			team_string=team_string+i;
			document.getElementById(team_string).value="";
			document.getElementById(team_string).disabled=true;
			document.getElementById(team_string).title="";
		}
	}
		
}

/**
 * @showFreq    刷新页面显示频率列表
 * @author		wdz
 */

function  showFreq()
{
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
	if(freq_channel==0)
	{
		for(var i=1;i<11;i++)
		{
			var freq_string='frePoint';
			freq_string=freq_string+i;
			document.getElementById(freq_string).disabled=true;
			document.getElementById(freq_string).title="";
		}
		return;
	}
	var invalidate_num=0;
	var language_switch=$("#language_switch").val();                    //语言开关   0：中文   1：英文
	for(var i=1;i<11;i++)
	{
		var freq_string='frePoint';
		freq_string=freq_string+i;
		
		if(validate_freq_item(freq_string)==0) 
		{
			invalidate_num=i;
			break;
		}
	}
	if(0==invalidate_num)
	{
		invalidate_num=11;
	}
	
	for(var i=1;i<invalidate_num;i++)
	{
		var freq_string='frePoint';
		freq_string=freq_string+i;
		var base_value=document.getElementById(freq_string).value;
		
		for(var j=i+1;j<invalidate_num;j++)
		{
			freq_string='frePoint';
			freq_string=freq_string+j;
			var current_value=document.getElementById(freq_string).value;
			
			if(parseFloat(base_value)== parseFloat(current_value))
			{
				invalidate_num=j;
				break;
			}
		}
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var freq_string='frePoint';
		freq_string=freq_string+i;
		document.getElementById(freq_string).disabled=false;
		
		if(0==language_switch)
		{
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400；输入格式：最多6位精度位数";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470；输入格式：最多6位精度位数";
				}
			}
		}
		else
		{                                                        
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{	
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. At most 6 decimal places are allowed";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{	
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. At most 6 decimal places are allowed";
				}
			}
		}
	}
	if(invalidate_num!=11)
	{
		var freq_string='frePoint';
		freq_string=freq_string+invalidate_num;
		document.getElementById(freq_string).value="";
		document.getElementById(freq_string).disabled=false;
		
		if(0==language_switch)
		{
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400；输入格式：最多6位精度位数";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470；输入格式：最多6位精度位数";
				}
			}
		}
		else
		{                                                        
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{	
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. At most 6 decimal places are allowed";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{	
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. At most 6 decimal places are allowed";
				}
			}
		}
		
		for(var i=invalidate_num+1;i<11;i++)
		{
				freq_string='frePoint';
				freq_string=freq_string+i;
				document.getElementById(freq_string).value="";
				document.getElementById(freq_string).disabled=true;
				document.getElementById(freq_string).title="";
		}
	}
			
}

/**
 * @showBlklist    刷新页面显示黑名单列表
 * @author		wdz
 */

function  showBlklist()
{
	var invalidate_num=0;
	var language_switch=$("#language_switch").val();            //语言开关   0：中文  1：英文
	for(var i=1;i<11;i++)
	{
		var Blklist_string='blkList';
		Blklist_string=Blklist_string+i;
		
		if(validate_blklist_item(Blklist_string)==0) 
		{
			invalidate_num=i;
			break;
		}
	}
	if(invalidate_num ==0)
	{
		invalidate_num=11;
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var blackNumber_string='blkList';
		blackNumber_string=blackNumber_string+i;
		var base_value=document.getElementById(blackNumber_string).value;
		
		for(var j=i+1;j<invalidate_num;j++)
		{
			blackNumber_string='blkList';
			blackNumber_string=blackNumber_string+j;
			var current_value=document.getElementById(blackNumber_string).value;
			
			if(base_value==current_value)
			{
				invalidate_num=j;
				break;
			}
		}
	}
	
	for(var i=1;i<invalidate_num;i++)
	{
		var Blklist_string='blkList';
		Blklist_string=Blklist_string+i;
		document.getElementById(Blklist_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(Blklist_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(Blklist_string).title="The number calls from which will be blocked. Enter at most 30 digits";
		}
	}
	if(invalidate_num!=11)
	{
		var Blklist_string='blkList';
		Blklist_string=Blklist_string+invalidate_num;
		document.getElementById(Blklist_string).value="";
		document.getElementById(Blklist_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(Blklist_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(Blklist_string).title="The number calls from which will be blocked. Enter at most 30 digits";
		}
		for(var i=invalidate_num+1;i<11;i++)
		{
			Blklist_string='blkList';
			Blklist_string=Blklist_string+i;
			document.getElementById(Blklist_string).value="";
			document.getElementById(Blklist_string).disabled=true;
			document.getElementById(Blklist_string).title="";
		}
		
	}
}

/**
 * @showWhtList    刷新页面显示白名单列表
 * @author		wdz
 */
function  showWhtList()
{
	var language_switch=$("#language_switch").val();        //语言开关   0：中文  1：英文
	var invalidate_num=0;
	
	for(var i=1;i<11;i++)
	{
		var whtList_string='whtList';
		whtList_string=whtList_string+i;
		
		if(validate_whtList_item(whtList_string)==0)
		{
			invalidate_num=i;
			break;
		}
	}
	
	if(invalidate_num==0)
	{
		invalidate_num=11;
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var whiteNumber_string='whtList';
		whiteNumber_string=whiteNumber_string+i;
		var base_value=document.getElementById(whiteNumber_string).value;
		
		for(var j=i+1;j<invalidate_num;j++)
		{
			whiteNumber_string='whtList';
			whiteNumber_string=whiteNumber_string+j;
			var current_value=document.getElementById(whiteNumber_string).value;
			
			if(base_value==current_value)
			{
				invalidate_num=j;
				break;
			}
		}
	}
	
	for(var i=1;i<invalidate_num;i++)
	{
		var whtList_string='whtList';
		whtList_string=whtList_string+i;
		document.getElementById(whtList_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(whtList_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(whtList_string).title="The number calls from which will be received. Enter at most 30 digits";
		}
	}
	if(invalidate_num!=11)
	{
		var whtList_string='whtList';
		whtList_string=whtList_string+invalidate_num;
		document.getElementById(whtList_string).value="";
		document.getElementById(whtList_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(whtList_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(whtList_string).title="The number calls from which will be received. Enter at most 30 digits";
		}
		
		for(var i=invalidate_num+1;i<11;i++)
		{
			whtList_string='whtList';
			whtList_string=whtList_string+i;
			document.getElementById(whtList_string).value="";
			document.getElementById(whtList_string).disabled=true;
			document.getElementById(whtList_string).title="";
		}
			
	}
	
}

/**
 * @showgsmComm    刷新页面显示gsm列表
 * @author		wdz
 */
function  showgsmComm()
{
	var language_switch=$("#language_switch").val();  //语言开关   0：中文  1：英文
	var invalidate_num=0;
	
	for(var i=1;i<11;i++)
	{
		var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+i;
		
		if(validate_gsmComm_item(gsmComm_string)==0)
		{
			invalidate_num=i;
			break;
		}
	}
	if(invalidate_num==0)
	{
		invalidate_num=11;
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var gsmNumber_string='gsmComm';
		gsmNumber_string=gsmNumber_string+i;
		var base_value=document.getElementById(gsmNumber_string).value;
		
		for(var j=i+1;j<invalidate_num;j++)
		{
			gsmNumber_string='gsmComm';
			gsmNumber_string=gsmNumber_string+j;
			var current_value=document.getElementById(gsmNumber_string).value;
			
			if(base_value==current_value)
			{
				invalidate_num=j;
				break;
			}
		}
	}
	for(var i=1;i<invalidate_num;i++)
	{
		var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+i;
		
		document.getElementById(gsmComm_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(gsmComm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
		}
		else
		{
			document.getElementById(gsmComm_string).title="GSM call number. Enter at most 30 characters. For example, +1234 or 1234";
		}
	}
	if(invalidate_num!=11)
	{
		var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+invalidate_num;
		document.getElementById(gsmComm_string).value="";
		document.getElementById(gsmComm_string).disabled=false;
		if(0==language_switch)
		{
			document.getElementById(gsmComm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
		}
		else
		{
			document.getElementById(gsmComm_string).title="GSM call number. Enter at most 30 characters. For example, +1234 or 1234";
		}
		for(var i=invalidate_num+1;i<11;i++)
		{
			gsmComm_string='gsmComm';
			gsmComm_string=gsmComm_string+i;
			document.getElementById(gsmComm_string).value="";
			document.getElementById(gsmComm_string).disabled=true;
			document.getElementById(gsmComm_string).title="";
		}
	}
}


/**
 * @impot    导入操作
 * @author		wdz
 */
function impot(){
   document.getElementById("import_down").value=1;          //置位为1，提交导入操作优先级，（组呼、频率、gsm、黑白名单无效情况可以点击导入操作）
   var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
   var language_switch=$("#language_switch").val();  //语言开关   0：中文  1：英文
   if(freq_channel==0)
   {
	   if(language_switch==0)
	   {
		   alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		   return;
	   }
	   else
	   {
		   alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		   return;
	   }
	   
   }
   //convert_data_to_def_import();                            //除了组呼、频率、gsm、黑白名单，其它输入无效均置位默认值；
   clear_invalidate_list_data();                            //清空组呼、频率、gsm、黑白名单中的无效数据
   $("#uploadfile").click();
}

/**
 * @createUploadFile    导入操作--读入文件内容
 * @author		wdz
 */
function createUploadFile() {
    var selectedFile = document.getElementById("uploadfile").files[0];//获取读取的File对象
    var name = selectedFile.name;//读取选中文件的文件名
    var size = selectedFile.size;//读取选中文件的大小
   // console.log("文件名:"+name+"大小："+size);

    var reader = new FileReader();//这里是核心！！！读取操作就是由它完成的。
    reader.readAsText(selectedFile);//读取文件的内容

    reader.onload = function(){
     //   console.log(this.result);//当读取完成之后会回调这个函数，然后此时文件的内容存储到了result中。直接操作即可。
     //  alert(this.result);
	    setConfParam(this.result);
    };

    $("#uploadfile").remove();
    var input = " <input type=\"file\" id=\"uploadfile\" name=\"uploadfile\" onchange=\"createUploadFile()\" />";
    $("#myupload").append(input);
       
}

/**
 * @setConfParam    分配文件内容
 * @author		wdz
 */
 
function setConfParam(confVal){
	/////解密/////
	var b = new Base64();  
	var confVal = b.decode(confVal); 
    var arr = confVal.split("=");//以=号作为分隔字符串
	var net_param_start=2;       
	var base_conf_start=8;
	var team_conf_start=33;
	var freq_conf_start=45;
	var gsm_start=57;
	var vertion_info_start=68;
	var normal_num=68+2+1+1;//
	var blk_list_start=71;
	var white_list_start=83;
	var midCovtConf_start=95;
	var engieer_info_start=109;
	var engineer_num=109+16+1+1;
	var index=confVal.lastIndexOf("=");
	var language_switch=$("#language_switch").val();
	if(index == -1)
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','导入文件格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The file format is wrong','Prompt');
		}
		return;
	}
	var sub_string=confVal.substring(0,index+1);
	var crc=crc32(sub_string);
	console.log(""+crc);
	console.log(""+arr.length+"aa"+normal_num+"bb"+engineer_num);
	if((crc != arr[arr.length-1])||(arr.length<normal_num))
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','导入文件格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The file format is wrong','Prompt');
		}
		return;
	}
	if(document.getElementById('current_mode').value==1)
	{
		if(arr[1]==0)
		{
			if(0==language_switch)
			{
				alarm_tip_import_file(2,'确定','取消','工程模式是否导入用户模式文件','温馨提示',arr);
			}
			else
			{
				alarm_tip_import_file_en(2,'OK','Cancel','You are in engeering mode. Are you sure you want to import configuration file of a normal user','Prompt',arr);
			}
			return;
		}
		else if(arr.length!=engineer_num)
		{
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','导入文件格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The file format is wrong','Prompt');
			}
			return;
		}
		
	}
    setNetParam(arr[net_param_start+1],arr[net_param_start+2],arr[net_param_start+3],arr[net_param_start+4],arr[net_param_start+5]);
    setBasConf(arr[base_conf_start+1],arr[base_conf_start+2],arr[base_conf_start+3],arr[base_conf_start+4],arr[base_conf_start+5],arr[base_conf_start+6],arr[base_conf_start+7],arr[base_conf_start+8],arr[base_conf_start+9],arr[base_conf_start+10],arr[base_conf_start+11],arr[base_conf_start+12],arr[base_conf_start+13],arr[base_conf_start+14]
	           ,arr[base_conf_start+15],arr[base_conf_start+16],arr[base_conf_start+17],arr[base_conf_start+18],arr[base_conf_start+19],arr[base_conf_start+20],arr[base_conf_start+21],arr[base_conf_start+22],arr[base_conf_start+23],arr[base_conf_start+24]);
    setTeamConf(arr[team_conf_start+1],arr[team_conf_start+2],arr[team_conf_start+3],arr[team_conf_start+4],arr[team_conf_start+5],arr[team_conf_start+6],arr[team_conf_start+7],arr[team_conf_start+8],arr[team_conf_start+9],arr[team_conf_start+10],arr[team_conf_start+11]);    
    setFreConf(arr[freq_conf_start+1],arr[freq_conf_start+2],arr[freq_conf_start+3],arr[freq_conf_start+4],arr[freq_conf_start+5],arr[freq_conf_start+6],arr[freq_conf_start+7],arr[freq_conf_start+8],arr[freq_conf_start+9],arr[freq_conf_start+10],arr[freq_conf_start+11]);
    setGsmConf(arr[gsm_start+1],arr[gsm_start+2],arr[gsm_start+3],arr[gsm_start+4],arr[gsm_start+5],arr[gsm_start+6],arr[gsm_start+7],arr[gsm_start+8],arr[gsm_start+9],arr[gsm_start+10]);
    
	setVertionConf(arr[vertion_info_start+1],arr[vertion_info_start+2]);
	if(document.getElementById('current_mode').value==1)
	{
		setBlkList(arr[blk_list_start+1],arr[blk_list_start+2],arr[blk_list_start+3],arr[blk_list_start+4],arr[blk_list_start+5],arr[blk_list_start+6],arr[blk_list_start+7],arr[blk_list_start+8],arr[blk_list_start+9],arr[blk_list_start+10],arr[blk_list_start+11]);
	    setWhtList(arr[white_list_start+1],arr[white_list_start+2],arr[white_list_start+3],arr[white_list_start+4],arr[white_list_start+5],arr[white_list_start+6],arr[white_list_start+7],arr[white_list_start+8],arr[white_list_start+9],arr[white_list_start+10],arr[white_list_start+11]);
		setMidCovtConf(arr[midCovtConf_start+1],arr[midCovtConf_start+2],arr[midCovtConf_start+3],arr[midCovtConf_start+4],arr[midCovtConf_start+5],arr[midCovtConf_start+6],arr[midCovtConf_start+7],arr[midCovtConf_start+8],arr[midCovtConf_start+9],arr[midCovtConf_start+10],arr[midCovtConf_start+11],arr[midCovtConf_start+12],arr[midCovtConf_start+13]);
		
		setEngineerConf(arr[engieer_info_start+1],arr[engieer_info_start+2],arr[engieer_info_start+3],arr[engieer_info_start+4],arr[engieer_info_start+5],arr[engieer_info_start+6],arr[engieer_info_start+7],arr[engieer_info_start+8],arr[engieer_info_start+9],arr[engieer_info_start+10],arr[engieer_info_start+11],arr[engieer_info_start+12]
		,arr[engieer_info_start+13],arr[engieer_info_start+14],arr[engieer_info_start+15],arr[engieer_info_start+16]);
	}
	if(0==language_switch)
	{
		alarm_tip(1,'确定','取消','导入文件成功','温馨提示');
	}
	else
	{
		alarm_tip(1,'OK','Cancel','File imported successfully','Prompt');
	}
	
}

/**
 * @setNetParam    分配文件内容--网络参数
 * @author		wdz
 */
function setNetParam(param1,param2,param3,param4,param5){
    document.getElementById('linkIP').value = param1;
    document.getElementById('mask').value = param2;
    document.getElementById('gateWay').value = param3;
    document.getElementById('centerIP').value = param4;
    document.getElementById('centerID').value = param5;
}

/**
 * @setBasConf    分配文件内容--基础设置
 * @author		wdz
 */
function setBasConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15,param16,param17,param18,param19,param20,param21,param22,param23,param24){
    document.getElementById('epkID').value = param1;
    document.getElementById('intercomID').value = param2;
    document.getElementById('power').value = param3;
    document.getElementById('epkCode').value = param4;
    document.getElementById('termCode').value = param5;
    document.getElementById('workMode').value = param6;
    document.getElementById('voiceCode').value = param7;
    
//  document.getElementById('termDizzyOverTime').value = param11;
    document.getElementById('muteThreshold').value = param8;
    document.getElementById('micVolume').value = param9;
    document.getElementById('gsmVolume').value = param10;
    document.getElementById('trumpetVolume').value = param11;
	document.getElementById('devCallTimeout').value = param12;
	document.getElementById('neighbor_switch_status').value = param13;
	document.getElementById('neighbor_period').value = param14;
	document.getElementById('neighbor_ai_switch').value = param15;
	document.getElementById('stop_transmit_switch').value = param16;
	document.getElementById('dev_language_switch').value = param17;
	document.getElementById('boot_mode_switch').value = param18;
	document.getElementById('reboot_strategy_switch').value = param19;
	
	document.getElementById('alarm_switch_status').value = param20;
	document.getElementById('alarm_count').value = param21;
	document.getElementById('alarm_show_switch').value = param22;
	document.getElementById('gsm_alarm_show_switch').value = param23;
	document.getElementById('radio_alarm_show_switch').value = param24;
	
}


/**
 * @setTeamConf    分配文件内容--组呼设置
 * @author		wdz
 */
function setTeamConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11){
    
    document.getElementById('team1').value = param2;
    document.getElementById('team2').value = param3;
    document.getElementById('team3').value = param4;
    document.getElementById('team4').value = param5;
    document.getElementById('team5').value = param6;
    document.getElementById('team6').value = param7;
    document.getElementById('team7').value = param8;
    document.getElementById('team8').value = param9;
    document.getElementById('team9').value = param10;
    document.getElementById('team10').value = param11; 
	
	showTeam();                                      //重新显示组呼列表（无效--disabled）
	add_id_option();                                 //更新当前组下拉列表
	document.getElementById('teamIdNow').value = param1;
	
}

/**
 * @setFreConf    分配文件内容--频率设置
 * @author		wdz
 */
function setFreConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11){
    
    document.getElementById('frePoint1').value = param2;
    document.getElementById('frePoint2').value = param3;
    document.getElementById('frePoint3').value = param4;
    document.getElementById('frePoint4').value = param5;
    document.getElementById('frePoint5').value = param6;
    document.getElementById('frePoint6').value = param7;
    document.getElementById('frePoint7').value = param8;
    document.getElementById('frePoint8').value = param9;
    document.getElementById('frePoint9').value = param10;
    document.getElementById('frePoint10').value = param11;
	
	showFreq();                                   //重新显示频率列表 （无效--disabled）
	add_freq_option();                            //更新当前组下拉列表
	document.getElementById('frePointNow').value = param1;
}


/**
 * @setBlkList    分配文件内容--黑名单设置
 * @author		wdz
 */
function setBlkList(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11){
    document.getElementById('blkListConf').value = param1;
    document.getElementById('blkList1').value = param2;
    document.getElementById('blkList2').value = param3;
    document.getElementById('blkList3').value = param4;
    document.getElementById('blkList4').value = param5;
    document.getElementById('blkList5').value = param6;
    document.getElementById('blkList6').value = param7;
    document.getElementById('blkList7').value = param8;
    document.getElementById('blkList8').value = param9;
    document.getElementById('blkList9').value = param10;
    document.getElementById('blkList10').value = param11;
	
	showBlklist();                           //重新显示黑名单列表（无效--disabled）
}


/**
 * @setWhtList    分配文件内容--白名单设置
 * @author		wdz
 */
function setWhtList(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11){
    document.getElementById('whtListConf').value = param1;
    document.getElementById('whtList1').value = param2;
    document.getElementById('whtList2').value = param3;
    document.getElementById('whtList3').value = param4;
    document.getElementById('whtList4').value = param5;
    document.getElementById('whtList5').value = param6;
    document.getElementById('whtList6').value = param7;
    document.getElementById('whtList7').value = param8;
    document.getElementById('whtList8').value = param9;
    document.getElementById('whtList9').value = param10;
    document.getElementById('whtList10').value = param11;
	
	showWhtList();                        //重新显示白名单列表（无效--disabled）
}

/**
 * @setGsmConf    分配文件内容--gsm设置
 * @author		wdz
 */
function setGsmConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10){
    document.getElementById('gsmComm1').value = param1;
    document.getElementById('gsmComm2').value = param2;
    document.getElementById('gsmComm3').value = param3;
    document.getElementById('gsmComm4').value = param4;
    document.getElementById('gsmComm5').value = param5;
    document.getElementById('gsmComm6').value = param6;
    document.getElementById('gsmComm7').value = param7;
    document.getElementById('gsmComm8').value = param8;
    document.getElementById('gsmComm9').value = param9;
    document.getElementById('gsmComm10').value = param10;
	
	showgsmComm();                       //重新显示gsm列表（无效--disabled）
}


/**
 * @setMidCovtConf    分配文件内容--中转台设置
 * @author		wdz
 */
function setMidCovtConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13){
	document.getElementById('radioIP').value = param1;
    document.getElementById('msgSlotTimePort1').value = param2;
    document.getElementById('msgSlotTimePort2').value = param3;
    document.getElementById('callCtrlSlotTimePort1').value = param4;
    document.getElementById('callCtrlSlotTimePort2').value = param5;
    document.getElementById('volumeSlotTimePort1').value = param6;
    document.getElementById('volumeSlotTimePort2').value = param7;
    document.getElementById('analogCallPort').value = param8;
    document.getElementById('analogVolumePort').value = param9;
    document.getElementById('radioID').value = param10;
    document.getElementById('interConctNum').value = param11;
    document.getElementById('msgCtrlAdmID').value = param12;
    document.getElementById('msgCtrlFilter').value = param13;
}

/**
 * @setVertionConf    分配文件内容--版本信息
 * @author		wdz
 */
function setVertionConf(param1,param2){
   document.getElementById('mac').value=param1;
   document.getElementById('version').value=param2;
}


/**
 * @setEngineerConf    分配文件内容--工程模式
 * @author		wdz
 */
//////////////////////工程模式//////////////////////
function setEngineerConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15,param16){
	document.getElementById('gsm_call_interval').value = param1;
    document.getElementById('gsm_reboot_time').value = param2;
    document.getElementById('vol_start_threshold').value = param3;
    document.getElementById('vol_delay').value = param4;
    document.getElementById('vol_energy_value').value = param5;
    document.getElementById('vol_packet_time').value = param6;
    document.getElementById('terminal_compatible').value = param7;
    document.getElementById('work_mode').value = param8;
    document.getElementById('voice_mode').value = param9;
	
	document.getElementById('linkTime').value = param10;
    document.getElementById('devOverTime').value = param11;
    document.getElementById('termGpsOverTime').value = param12;
	
	document.getElementById('close_transmit_rssi_threshold').value = param13;
	document.getElementById('resume_transmit_rssi_threshold').value = param14;
	document.getElementById('tempratue_alarm_start_threshold').value = param15;
	document.getElementById('tempratue_alarm_close_threshold').value = param16;
}


function crc32 (str) {
    var table = "00000000 77073096 EE0E612C 990951BA 076DC419 706AF48F E963A535 9E6495A3 0EDB8832 79DCB8A4 E0D5E91E 97D2D988 09B64C2B 7EB17CBD E7B82D07 90BF1D91 1DB71064 6AB020F2 F3B97148 84BE41DE 1ADAD47D 6DDDE4EB F4D4B551 83D385C7 136C9856 646BA8C0 FD62F97A 8A65C9EC 14015C4F 63066CD9 FA0F3D63 8D080DF5 3B6E20C8 4C69105E D56041E4 A2677172 3C03E4D1 4B04D447 D20D85FD A50AB56B 35B5A8FA 42B2986C DBBBC9D6 ACBCF940 32D86CE3 45DF5C75 DCD60DCF ABD13D59 26D930AC 51DE003A C8D75180 BFD06116 21B4F4B5 56B3C423 CFBA9599 B8BDA50F 2802B89E 5F058808 C60CD9B2 B10BE924 2F6F7C87 58684C11 C1611DAB B6662D3D 76DC4190 01DB7106 98D220BC EFD5102A 71B18589 06B6B51F 9FBFE4A5 E8B8D433 7807C9A2 0F00F934 9609A88E E10E9818 7F6A0DBB 086D3D2D 91646C97 E6635C01 6B6B51F4 1C6C6162 856530D8 F262004E 6C0695ED 1B01A57B 8208F4C1 F50FC457 65B0D9C6 12B7E950 8BBEB8EA FCB9887C 62DD1DDF 15DA2D49 8CD37CF3 FBD44C65 4DB26158 3AB551CE A3BC0074 D4BB30E2 4ADFA541 3DD895D7 A4D1C46D D3D6F4FB 4369E96A 346ED9FC AD678846 DA60B8D0 44042D73 33031DE5 AA0A4C5F DD0D7CC9 5005713C 270241AA BE0B1010 C90C2086 5768B525 206F85B3 B966D409 CE61E49F 5EDEF90E 29D9C998 B0D09822 C7D7A8B4 59B33D17 2EB40D81 B7BD5C3B C0BA6CAD EDB88320 9ABFB3B6 03B6E20C 74B1D29A EAD54739 9DD277AF 04DB2615 73DC1683 E3630B12 94643B84 0D6D6A3E 7A6A5AA8 E40ECF0B 9309FF9D 0A00AE27 7D079EB1 F00F9344 8708A3D2 1E01F268 6906C2FE F762575D 806567CB 196C3671 6E6B06E7 FED41B76 89D32BE0 10DA7A5A 67DD4ACC F9B9DF6F 8EBEEFF9 17B7BE43 60B08ED5 D6D6A3E8 A1D1937E 38D8C2C4 4FDFF252 D1BB67F1 A6BC5767 3FB506DD 48B2364B D80D2BDA AF0A1B4C 36034AF6 41047A60 DF60EFC3 A867DF55 316E8EEF 4669BE79 CB61B38C BC66831A 256FD2A0 5268E236 CC0C7795 BB0B4703 220216B9 5505262F C5BA3BBE B2BD0B28 2BB45A92 5CB36A04 C2D7FFA7 B5D0CF31 2CD99E8B 5BDEAE1D 9B64C2B0 EC63F226 756AA39C 026D930A 9C0906A9 EB0E363F 72076785 05005713 95BF4A82 E2B87A14 7BB12BAE 0CB61B38 92D28E9B E5D5BE0D 7CDCEFB7 0BDBDF21 86D3D2D4 F1D4E242 68DDB3F8 1FDA836E 81BE16CD F6B9265B 6FB077E1 18B74777 88085AE6 FF0F6A70 66063BCA 11010B5C 8F659EFF F862AE69 616BFFD3 166CCF45 A00AE278 D70DD2EE 4E048354 3903B3C2 A7672661 D06016F7 4969474D 3E6E77DB AED16A4A D9D65ADC 40DF0B66 37D83BF0 A9BCAE53 DEBB9EC5 47B2CF7F 30B5FFE9 BDBDF21C CABAC28A 53B39330 24B4A3A6 BAD03605 CDD70693 54DE5729 23D967BF B3667A2E C4614AB8 5D681B02 2A6F2B94 B40BBE37 C30C8EA1 5A05DF1B 2D02EF8D";

    var crc = 0;
    var x = 0;
    var y = 0;
    crc = crc ^ (-1);
    for( var i = 0, iTop = str.length; i < iTop; i++ ) 
	{
        y = ( crc ^ str.charCodeAt( i ) ) & 0xFF;
        x = "0x" + table.substr( y * 9, 8 );
        crc = ( crc >> 8 ) ^ x;
    }
    return crc ^ (-1);
};


////////////////////////////导出////////////////////////////////////////////
/**
 * @expt    导出文件
 * @author		wdz
 */
function expt(){
    try {
        var isFileSaverSupported = !!new Blob;
    } catch (e) {  alert("该版本浏览器不支持导出！"); }
	
	var language_switch=$("#language_switch").val();   //0：中文；1：英文
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
	if(freq_channel==0)
	{
	   if(language_switch==0)
	   {
		   alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		   return;
	   }
	   else
	   {
		   alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		   return;
	   }
	   
	}
	if(validate_linkIp()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机IP输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Local IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_linkIp_add()==0)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机IP不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Local IP Address cannot contain extra zeros','Prompt');
		}
		return ;
    }
	if(validate_mask()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Subnet Mask is wrong','Prompt');
		}
		return;
    }
	if(validate_mask_add()==0)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot contain extra zeros','Prompt');
		}
		return;
    }
	var  mask_return =validate_mask_value();
	if(mask_return ==1)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能为0.0.0.0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot be 0.0.0.0','Prompt');
		}
		return ;
    }
	else if(2==mask_return)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码不能为255.255.255.255','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Subnet Mask cannot be 255.255.255.255','Prompt');
		}
		return ;
	}
	if(validate_mask_continue()==0)
	{
		showDiv(1);
		document.getElementById('mask').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码无效，子网掩码必须相邻','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Invalid subnet mask. The subnet mask has to be contiguous','Prompt');
		}
		return;
    }
	var mask_and_ip=validate_mask_and_ip();
	if(mask_and_ip==1)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与本机IP组合无效，本机IP主机地址所有位不能均为0','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and local IP address is invalid. It is disallowed that all of the bits in the host address portion of the IP address are set to 0','Prompt');
		}
		return;
    }
	else if(mask_and_ip==2)
	{
		showDiv(1);
		document.getElementById('linkIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与本机IP组合无效，本机IP主机地址所有位不能均为1','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','The combination of subnet mask and local IP address is invalid. It is disallowed that all of the bits in the host address portion of the IP address are set to 1','Prompt');
		}
		return;
	}
		
	if(validate_gateWay()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Gateway IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_gateWay_add()==0)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Gateway IP Address cannot contain extra zeros','Prompt');
		}
		return;
    }
	if(validate_gateWay_value()==0)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','网关与本机IP不在同一个网段','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Gateway IP Address and Local IP Address are in different network segments','Prompt');
		}
		return;
    }
	var mask_and_gateway=validate_mask_and_gateway();
	if(mask_and_gateway==1)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与网关组合无效，网关主机地址所有位不能均为0','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The combination of subnet mask and gateway IP address is invalid. It is disallowed that all of the bits in the host address portion of the gateway IP address are set to 0','Prompt');
		}
		return;
    }
	else if(mask_and_gateway==2)
	{
		showDiv(1);
		document.getElementById('gateWay').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','子网掩码与网管组合无效，网关主机地址所有位不能均为1','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The combination of subnet mask and gateway IP address is invalid. It is disallowed that all of the bits in the host address portion of the gateway IP address are set to 1','Prompt');
		}
		return;
	}
	if(validate_centerIP()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中心IP输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Center IP Address is wrong','Prompt');
		}
		return ;
    }
	if(validate_centerIP_add()==0)
	{
		showDiv(1);
		document.getElementById('centerIP').style.borderColor="red";
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中心IP不能包含多余的0','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Center IP address cannot contain extra zeros','Prompt');
		}
		return ;
    }
	if(validate_centerID()==0)
	{
		showDiv(1);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','互联ID输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Interconnection ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_epkID()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','本机编号输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Local Device ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_intercomID()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','对讲ID输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Radio ID is wrong','Prompt');
		}
		return ;
    }
	if(validate_power_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','发射功率输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of TX Power is wrong','Prompt');
		}
		return ;
    }
	if(validate_epkCode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','链路色码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Link Color Code is wrong','Prompt');
		}
		return ;
    }
	if(validate_termCode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','终端色码输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Radio Color Code is wrong','Prompt');
		}
		return ;
    }
	if(validate_code()== false)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','终端色码应不同于链路色码','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The Link Color Code and Radio Color Code must be different','Prompt');
		}
		return ;
	}
	
	if(validate_muteThreshold_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','静噪门限输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Squelch Threshold is wrong','Prompt');
		}
		return ;
	}
	if(validate_micVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','MIC音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of MIC Volume is wrong','Prompt');
		}
		
		return ;
	}
	if(validate_gsmVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','GSM音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of GSM Volume is wrong','Prompt');
		}
		return ;
	}
	if(validate_trumpetVolume_value()== 0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','手咪默认音量输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Palm Microphone Volume is wrong','Prompt');
		}
		return ;
	}
	if(validate_devCallTimeout()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','通话限时输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Call Duration Limit is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_switch_status_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点查询使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Neighbor Query is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_period_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点周期输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Neighbor Query Interval is wrong','Prompt');
		}
		return ;
    }
	if(validate_neighbor_ai_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','邻点上报使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Report Neighbor to Center is wrong','Prompt');
		}
		return ;
    }
	if(validate_stop_transmit()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','智能中转输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Smart Repeating is wrong','Prompt');
		}
		return ;
    }
	if(validate_dev_language_switch()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','语言开关输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Language Switch is wrong','Prompt');
		}
		return ;
    }
	if(validate_boot_mode()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','开机方式输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Boot Method is wrong','Prompt');
		}
		return ;
    }
	if(validate_reboot_strategy_switch()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','启动策略输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'确定','取消','The format of Startup Policy is wrong','温馨提示');
		}
		return ;
    }
	if(validate_alarm_switch_status_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','告警上报使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Alarm Reporting is wrong','Prompt');
		}
		return ;
    }
	
	if(validate_alarm_count()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','紧急警报重复次数输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The format of Emergency Alarm Transmissions is wrong','Prompt');
		}
		return ;
    }
	if(validate_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','紧急警报显示使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Display Emergency Alarm is wrong','Prompt');
		}
		
		return ;
    }
	if(validate_gsm_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','GSM告警使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of GSM Alarm is wrong','Prompt');
		}
		return ;
    }
	if(validate_radio_alarm_show_switch_value()==0)
	{
		showDiv(2);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','中转台告警使能输入格式错误','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The input of Repeater Alarm is wrong','Prompt');
		}
		
		return ;
    }
	//////////////////验证组呼号////////////////////////////
	var team_return=before_write_validate_groupNumber();
	if(team_return==0)
	{	
		return ;
	}
	if(validate_IdNow_in_team()==false)
	{
		showDiv(3);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前组不在组1~组10列表中','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The current group is out of Groups 1-10','Prompt');
		}
		return ;
	}
	var freq_return=before_write_validate_freqlist();
	if(freq_return==0)
	{
		return ;
	}
	if(validate_freqNow_in_team()==false)
	{
		showDiv(4);
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前频率不在频率1~频率10列表中','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The current frequency is out of Frequencies 1-10','Prompt');
		}
		return ;
	}
	
	var gsmComm_return=before_write_validate_gsmNumber();
	if(gsmComm_return0=0)
	{
		return ;	
	}
	
	////////////////工程模式//////////////////////
	if(document.getElementById("current_mode").value==1)    //0：代表用户模式  1：代表工程模式
	{
		if(validate_blkListConf_value()==0)
		{
			showDiv(8);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','黑名单使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Blacklist is wrong','Prompt');
			}
			return ;
		}
		
		var blklist_return=before_write_validate_blackNumber();
		if(blklist_return==0)
		{
			return ;
		}
		if(validate_whtListConf_value()==0)
		{
			showDiv(8);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','白名单使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Whitelist is wrong','Prompt');
			}
			return ;
		}
		var whtList_return=before_write_validate_whiteNumber();
		if(whtList_return==0)
		{
			return ;
		}
	
		if(validate_radioIP()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台IP输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater IP Address is wrong','Prompt');
			}
			return ;
		}
		if(validate_radioIP_add()==0)
		{
			showDiv(10);
			document.getElementById('radioIP').style.borderColor="red";
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台IP不能包含多余的0','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The Repeater IP Address cannot contain extra zeros','Prompt');
			}
			return ;
		}
		if(validate_radioID()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转台ID输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater ID is wrong','Prompt');
			}
			return ;
		}
		if(validate_interConctNum()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','中转组输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeater Group is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_msgCtrlAdmID()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','短信控制管理员ID输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Message Manager ID is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_msgCtrlFilter_value()==0)
		{
			showDiv(10);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','短信控制过滤使能输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Message Filter is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_gsm_call_interval()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','GSM主叫重呼间隔输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of GSM Recall Interval is wrong','Prompt');
			}
			return ;
		}
	
		if(validate_gsm_reboot_time()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','GSM模块异常重启时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of GSM Fault Recovery Interval is wrong','Prompt');
			}
			return ;
		}
		if(validate_vol_start_threshold_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测启动门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Detection Start Threshold is wrong','Prompt');
			}
			
			return ;
		}
		if(validate_vol_delay()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测拖尾延时输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Speak Right Reset Delay is wrong','Prompt');
			}
			return ;
		}
	
		if(validate_vol_energy_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音检测能量阈值输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Detection Energy Threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_vol_packet_time_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音包时长输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Voice Packet Length is wrong','Prompt');
			}
			return ;
		}
		if(validate_terminal_compatible_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','终端兼容模式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Support Motorola Radios is wrong','Prompt');
			}
			return ;
		}
		if(validate_work_mode_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','工作模式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Operation Mode is wrong','Prompt');
			}
			return ;
		}
		if(validate_voice_mode_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','语音编码格式输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The input of Voice Codec is wrong','Prompt');
			}
			return ;
		}
		if(validate_linkTime_value()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','链路保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Link Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_devOverTime()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','链路业务保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Link Service Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_termGpsOverTime()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','终端业务保持时间输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Radio Service Hold Time is wrong','Prompt');
			}
			return ;
		}
		if(validate_close_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','关闭转发场强门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeating Stop RSSI threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_resume_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','恢复转发场强门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Repeating Recover RSSI threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_tempratue_alarm_start_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','温度告警门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Temperature Alarm Threshold is wrong','Prompt');
			}
			return ;
		}
		if(validate_tempratue_alarm_close_threshold()==0)
		{
			showDiv(11);
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','温度告警恢复门限输入格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of Temperature Alarm Recovery Threshold is wrong','Prompt');
			}
			return ;
		}
	}
	var mode = getCurMode();
    var netParam = getNetParam();
    var basConf = getBasConf();
    var teamConf = getTeamConf();
    var freConf = getFreConf();
    var gsmConf = getGsmConf();
	var vertion_info=getVertionConf();
	
	if(document.getElementById('current_mode').value==1)
	{
		var blkList = getBlkList();
		var whtList = getWhtList();
		var midCovtConf = getMidCovtConf();
		var engineer_info=getEngineerConf();
		var str =mode+netParam+basConf+teamConf+freConf+gsmConf+vertion_info+blkList+whtList+midCovtConf+engineer_info;
		var crc=crc32(str);
		var str=str+crc;
		var b = new Base64();  
        var str_code = b.encode(str);
		var blob = new Blob([str_code], {type: "text/plain;charset=utf-8"});
	}
	else
	{
		var str =mode+netParam+basConf+teamConf+freConf+gsmConf+vertion_info;
		var crc=crc32(str);
		var str=str+crc;
		var b = new Base64();  
        var str_code = b.encode(str);
		var blob = new Blob([str_code], {type: "text/plain;charset=utf-8"});
	}
    
    saveAs(blob, "EpackConfig.txt");   
}


//获取各配置模块参数方法

/**
 * @getCurMode    导出文件增加当前模式信息（0：用户模式  1：工程模式），用以区分导出文件类型
 * @author		wdz
 */
function getCurMode(){
    var mode =document.getElementById('current_mode').value;
    var retVal = "mode:="+mode+"=";
    return retVal;
}

/**
 * @getNetParam    导出文件增加--网络参数
 * @author		wdz
 */
function getNetParam(){
    var linkIP = document.getElementById('linkIP').value;
    var mask = document.getElementById('mask').value;
    var gateWay = document.getElementById('gateWay').value;
    var centerIP = document.getElementById('centerIP').value;
    var centerID = document.getElementById('centerID').value;
    var retVal = "web_conf:="+linkIP+"="+mask+"="+gateWay+"="+centerIP+"="+centerID+"=";
    return retVal;
}

/**
 * @getBasConf    导出文件增加--基本设置
 * @author		wdz
 */
function getBasConf(){
    var epkID = document.getElementById('epkID').value;
    var intercomID = document.getElementById('intercomID').value;
    var power = document.getElementById('power').value;
    var epkCode = document.getElementById('epkCode').value;
    var termCode = document.getElementById('termCode').value;
	var workMode = document.getElementById('workMode').value;
    var voiceCode = document.getElementById('voiceCode').value;
    
 //   var termDizzyOverTime = document.getElementById('termDizzyOverTime').value;
    var muteThreshold = document.getElementById('muteThreshold').value;
    var micVolume = document.getElementById('micVolume').value;
    var gsmVolume = document.getElementById('gsmVolume').value;
    var trumpetVolume = document.getElementById('trumpetVolume').value;
		
	var devCallTimeout=document.getElementById('devCallTimeout').value;
	var neighbor_switch_status=document.getElementById('neighbor_switch_status').value;
	var neighbor_period=document.getElementById('neighbor_period').value;
	var neighbor_ai_switch=document.getElementById('neighbor_ai_switch').value;
	var stop_transmit_switch=document.getElementById('stop_transmit_switch').value;
	var dev_language_switch=document.getElementById('dev_language_switch').value;
	var boot_mode_switch=document.getElementById('boot_mode_switch').value;
	var reboot_strategy_switch=document.getElementById('reboot_strategy_switch').value;
	
	var alarm_switch_status=document.getElementById('alarm_switch_status').value;
	var alarm_count=document.getElementById('alarm_count').value;
	var alarm_show_switch=document.getElementById('alarm_show_switch').value;
	var gsm_alarm_show_switch=document.getElementById('gsm_alarm_show_switch').value;
	var radio_alarm_show_switch=document.getElementById('radio_alarm_show_switch').value ;
	
    var retVal = "bas_conf:="+epkID+"="+intercomID+"="+power+"="+epkCode+"="+termCode+"="+workMode+"="+voiceCode+"="+muteThreshold+"="+micVolume+"="+gsmVolume+"="+trumpetVolume+"="+
				devCallTimeout+"="+neighbor_switch_status+"="+neighbor_period+"="+neighbor_ai_switch+"="+stop_transmit_switch+"="+dev_language_switch+"="+boot_mode_switch+"="+reboot_strategy_switch+"="+alarm_switch_status+"="+alarm_count+"="+alarm_show_switch+"="+gsm_alarm_show_switch+"="+
				radio_alarm_show_switch+"=";
    return retVal;
}

/**
 * @getTeamConf    导出文件增加--组呼设置
 * @author		wdz
 */
function getTeamConf(){
    var teamIdNow = document.getElementById('teamIdNow').value;
    var team1 = document.getElementById('team1').value;
    var team2 = document.getElementById('team2').value;
    var team3 = document.getElementById('team3').value;
    var team4 = document.getElementById('team4').value;
    var team5 = document.getElementById('team5').value;
    var team6 = document.getElementById('team6').value;
    var team7 = document.getElementById('team7').value;
    var team8 = document.getElementById('team8').value;
    var team9 = document.getElementById('team9').value;
    var team10 = document.getElementById('team10').value;
    var retVal = "tea_conf:="+teamIdNow+"="+team1+"="+team2+"="+team3+"="+team4+"="+team5+"="+team6+"="+team7+"="+team8+"="+team9+"="+team10+"=";
    return retVal;
}


/**
 * @getFreConf    导出文件增加--频率设置
 * @author		wdz
 */
function getFreConf(){
    var frePointNow = document.getElementById('frePointNow').value;
    var frePoint1 = document.getElementById('frePoint1').value;
    var frePoint2 = document.getElementById('frePoint2').value;
    var frePoint3 = document.getElementById('frePoint3').value;
    var frePoint4 = document.getElementById('frePoint4').value;
    var frePoint5 = document.getElementById('frePoint5').value;
    var frePoint6 = document.getElementById('frePoint6').value;
    var frePoint7 = document.getElementById('frePoint7').value;
    var frePoint8 = document.getElementById('frePoint8').value;
    var frePoint9 = document.getElementById('frePoint9').value;
    var frePoint10 = document.getElementById('frePoint10').value;
    var retVal = "freq_conf:="+frePointNow+"="+frePoint1+"="+frePoint2+"="+frePoint3+"="+frePoint4+"="+frePoint5+"="+frePoint6+"="+frePoint7+"="+frePoint8+"="+frePoint9+"="+frePoint10+"=";
    return retVal;
}


/**
 * @getBlkList    导出文件增加--黑名单设置
 * @author		wdz
 */
function getBlkList(){
    var blkListConf = document.getElementById('blkListConf').value;
    var blkList1 = document.getElementById('blkList1').value;
    var blkList2 = document.getElementById('blkList2').value;
    var blkList3 = document.getElementById('blkList3').value;
    var blkList4 = document.getElementById('blkList4').value;
    var blkList5 = document.getElementById('blkList5').value;
    var blkList6 = document.getElementById('blkList6').value;
    var blkList7 = document.getElementById('blkList7').value;
    var blkList8 = document.getElementById('blkList8').value;
    var blkList9 = document.getElementById('blkList9').value;
    var blkList10 = document.getElementById('blkList10').value;
    var retVal = "blk_conf:="+blkListConf+"="+blkList1+"="+blkList2+"="+blkList3+"="+blkList4+"="+blkList5+"="+blkList6+"="+blkList7+"="+blkList8+"="+blkList9+"="+blkList10+"=";
    return retVal;
}

/**
 * @getWhtList    导出文件增加--白名单设置
 * @author		wdz
 */

function getWhtList(){
    var whtListConf = document.getElementById('whtListConf').value;
    var whtList1 = document.getElementById('whtList1').value;
    var whtList2 = document.getElementById('whtList2').value;
    var whtList3 = document.getElementById('whtList3').value;
    var whtList4 = document.getElementById('whtList4').value;
    var whtList5 = document.getElementById('whtList5').value;
    var whtList6 = document.getElementById('whtList6').value;
    var whtList7 = document.getElementById('whtList7').value;
    var whtList8 = document.getElementById('whtList8').value;
    var whtList9 = document.getElementById('whtList9').value;
    var whtList10 = document.getElementById('whtList10').value;
    var retVal = "white_conf:="+whtListConf+"="+whtList1+"="+whtList2+"="+whtList3+"="+whtList4+"="+whtList5+"="+whtList6+"="+whtList7+"="+whtList8+"="+whtList9+"="+whtList10+"=";
    return retVal;
}


/**
 * @getGsmConf    导出文件增加--gsm设置
 * @author		wdz
 */
function getGsmConf(){
    var gsmComm1 = document.getElementById('gsmComm1').value;
    var gsmComm2 = document.getElementById('gsmComm2').value;
    var gsmComm3 = document.getElementById('gsmComm3').value;
    var gsmComm4 = document.getElementById('gsmComm4').value;
    var gsmComm5 = document.getElementById('gsmComm5').value;
    var gsmComm6 = document.getElementById('gsmComm6').value;
    var gsmComm7 = document.getElementById('gsmComm7').value;
    var gsmComm8 = document.getElementById('gsmComm8').value;
    var gsmComm9 = document.getElementById('gsmComm9').value;
    var gsmComm10 = document.getElementById('gsmComm10').value;
    var retVal = "gsm_conf:="+gsmComm1+"="+gsmComm2+"="+gsmComm3+"="+gsmComm4+"="+gsmComm5+"="+gsmComm6+"="+gsmComm7+"="+gsmComm8+"="+gsmComm9+"="+gsmComm10+"=";
    return retVal;
}


/**
 * @getMidCovtConf    导出文件增加--中转台设置
 * @author		wdz
 */
function getMidCovtConf(){
	var radioIP= document.getElementById('radioIP').value;
    var msgSlotTimePort1 = document.getElementById('msgSlotTimePort1').value;
    var msgSlotTimePort2 = document.getElementById('msgSlotTimePort2').value;
    var callCtrlSlotTimePort1 = document.getElementById('callCtrlSlotTimePort1').value;
    var callCtrlSlotTimePort2 = document.getElementById('callCtrlSlotTimePort2').value;
    var volumeSlotTimePort1 = document.getElementById('volumeSlotTimePort1').value;
    var volumeSlotTimePort2 = document.getElementById('volumeSlotTimePort2').value;
    var analogCallPort = document.getElementById('analogCallPort').value;
    var analogVolumePort = document.getElementById('analogVolumePort').value;
    var radioID = document.getElementById('radioID').value;
    var interConctNum = document.getElementById('interConctNum').value;
    var msgCtrlAdmID = document.getElementById('msgCtrlAdmID').value;
    var msgCtrlFilter = document.getElementById('msgCtrlFilter').value;
    var retVal = "mid_conf:="+radioIP+"="+msgSlotTimePort1+"="+msgSlotTimePort2+"="+callCtrlSlotTimePort1+"="+callCtrlSlotTimePort2+"="+volumeSlotTimePort1+"="+volumeSlotTimePort2+"="+analogCallPort+"="+analogVolumePort+"="+radioID+"="+interConctNum+"="+msgCtrlAdmID+"="+msgCtrlFilter+"=";
    return retVal;
}



/**
 * @getVertionConf    导出文件增加--版本信息/////////////////////////////
 * @author		wdz
 */
function getVertionConf(){
	var mac = document.getElementById('mac').value;
    var epack_vertion = document.getElementById('version').value;
    var retVal = "ver_conf:="+mac+"="+epack_vertion+"=";
    return retVal;
}


/**
 * @getEngineerConf    导出文件增加--工程模式
 * @author		wdz
 */
function getEngineerConf(){
	
	var gsm_call_interval = document.getElementById('gsm_call_interval').value;
	var gsm_reboot_time = document.getElementById('gsm_reboot_time').value;
	var vol_start_threshold = document.getElementById('vol_start_threshold').value;
	var vol_delay = document.getElementById('vol_delay').value;
	var vol_energy_value = document.getElementById('vol_energy_value').value;
	var vol_packet_time = document.getElementById('vol_packet_time').value;
	var terminal_compatible = document.getElementById('terminal_compatible').value;
	var work_mode = document.getElementById('work_mode').value;
	var voice_mode = document.getElementById('voice_mode').value;
	
	var linkTime = document.getElementById('linkTime').value;
    var devOverTime = document.getElementById('devOverTime').value;
    var termGpsOverTime = document.getElementById('termGpsOverTime').value;
	
	var close_transmit_rssi_threshold=document.getElementById('close_transmit_rssi_threshold').value;
	var resume_transmit_rssi_threshold=document.getElementById('resume_transmit_rssi_threshold').value;
	var tempratue_alarm_start=document.getElementById('tempratue_alarm_start_threshold').value;
	var tempratue_alarm_close=document.getElementById('tempratue_alarm_close_threshold').value;
	
    var retVal = "eng_conf:="+gsm_call_interval+"="+gsm_reboot_time+"="+vol_start_threshold+"="+vol_delay+"="+vol_energy_value+"="+vol_packet_time+"="+terminal_compatible+"="+work_mode+"="+voice_mode+"="
								+linkTime+"="+devOverTime+"="+termGpsOverTime+"="+close_transmit_rssi_threshold+"="+resume_transmit_rssi_threshold+"="+tempratue_alarm_start+"="+tempratue_alarm_close+"=";
    return retVal;
}
////////////////////////////导出////////////////////////////////////////////

/**
 * @showOption    刷新页面将所有下拉列表置位
 * @author		wdz
 */
function showOption(dev_power,linkTime,half_variance_threshold,mic_volume,gsm_volume, horn_volume,blacklist_switch,whitelist_switch,filter_switch,
					alarm_switch_status,neighbor_switch,alarm_show_switch,gsm_alarm_show_switch,radio_alarm_show_switch,neighbor_ai_switch,current_id,current_freq,
					vol_start_threshold,vol_packet_time,terminal_compatible,work_mode_2,vol_code_2,neighbor_period,stop_tansmit,device_language_switch,boot_mode,reboot_strategy_switch){
	document.getElementById('power').value =dev_power;
    document.getElementById('muteThreshold').value = half_variance_threshold;
    document.getElementById('micVolume').value = mic_volume;
    document.getElementById('gsmVolume').value = gsm_volume;
    document.getElementById('trumpetVolume').value = horn_volume;
	document.getElementById('alarm_switch_status').value=alarm_switch_status;
	document.getElementById('neighbor_switch_status').value=neighbor_switch;
	document.getElementById('alarm_show_switch').value=alarm_show_switch;
	document.getElementById('gsm_alarm_show_switch').value=gsm_alarm_show_switch;
	document.getElementById('radio_alarm_show_switch').value=radio_alarm_show_switch;
	document.getElementById('neighbor_ai_switch').value=neighbor_ai_switch;
	document.getElementById('neighbor_period').value=neighbor_period;
	document.getElementById('stop_transmit_switch').value=stop_tansmit;
	document.getElementById('dev_language_switch').value=device_language_switch;
	document.getElementById('boot_mode_switch').value=boot_mode;
	document.getElementById('reboot_strategy_switch').value=reboot_strategy_switch;
	add_id_option();
	document.getElementById('teamIdNow').value=current_id;
	add_freq_option();
	document.getElementById('frePointNow').value=current_freq;
	
	///////////////工程模式/////////////////////////////////
	if(document.getElementById('current_mode').value==1)
	{
		document.getElementById('blkListConf').value = blacklist_switch;
		document.getElementById('whtListConf').value=whitelist_switch;
		document.getElementById('linkTime').value = linkTime;
		document.getElementById('msgCtrlFilter').value=filter_switch;
		document.getElementById('vol_start_threshold').value=vol_start_threshold;
		document.getElementById('vol_packet_time').value=vol_packet_time;
		document.getElementById('terminal_compatible').value=terminal_compatible;
		document.getElementById('work_mode').value=work_mode_2;
		document.getElementById('voice_mode').value=vol_code_2;
	}
	
	///////////////工程模式/////////////////////////////////
}

/**
 * @showOption    每次操作完成返回结果
 * @author		wdz
 */
function showTip(remind){
	
	var language_switch=$("#language_switch").val();        //语言开关  0：中文  1：英文
	if(remind== 1)                                          // 1：重启成功
	{
		RepeatWrite();
	//	alarm_tip_reboot_log(1,'确定','取消','重启成功，请重新登录','温馨提示');	
	}
	else if(remind== 2)                                   // 2：重启失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','重启失败','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Restart failed','Prompt');
		}
	}
	else if(remind== 3)                                //3：读取成功
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','读取成功','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Read succeeded','Prompt');
		}
	}
	else if(remind== 4)                              // 4：读取失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','读取失败','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Read failed','Prompt');
		}
	}
	else if(remind== 5)                             //5：写入成功，触发重启
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("reboot_operate").click();
		if(0==language_switch)
		{
			layer.load('配置成功，正在重启。。。。');
		}
		else
		{
			layer.load('Configuration completed successfully. Restarting...');
		}
		
	}
	else if(remind== 6)                          //6：写入失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','配置失败','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Configuration failed','Prompt');
		}
	}
	else if(remind== 7)                          //7：设备树升级成功  ，触发重启
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','设备树升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','Device tree upgraded successfully. Do you want to restart?','Prompt');
		}
		
	} 
	else if(remind== 8)                         //8：设备树升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','设备树升级失败，重启或关闭设备前务必保证设备树升级成功','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Device tree upgrade failed. Make sure the device tree is upgraded successfully before restarting or switching off the device','Prompt');
		}
	}
	else if(remind== 9)                        //9：FPGA程序升级成功，触发重启
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','FPGA程序升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','FPGA program upgraded successfully. Do you want to restart?','Prompt');
		}
		
	}
	else if(remind== 10)                     //10：FPGA程序升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','FPGA程序升级失败，重启或关闭设备前务必保证FPGA程序升级成功','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','FPGA program upgrade failed. Make sure the FPGA program is upgraded successfully before restarting or switching off the device','Prompt');
		}
	}
	else if(remind== 11)                   //11：boot升级成功，触发重启操作
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','BOOT升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','BOOT upgraded successfully. Do you want to restart?','Prompt');
		}
	}
	else if(remind== 12)                  //12：BOOT升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','BOOT升级失败，重启或关闭设备前务必保证BOOT升级成功','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','BOOT upgrade failed. Make sure the BOOT is upgraded successfully before restarting or switching off the device','Prompt');
		}
		
	}
	else if(remind== 13)                    //13：应用程序升级成功，触发重启
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','应用程序升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','Applications upgraded successfully. Do you want to restart?','Prompt');
		}
		
	}
	else if(remind== 14)                //14：应用程序升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','应用程序升级失败','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Applications upgrade failed','Prompt');
		}
		
	} 
	else if(remind== 15)               //15：文件系统升级成功，触发重启
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','文件系统升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','File system upgraded successfully. Do you want to restart?','Prompt');
		}
		
	}
	else if(remind== 16)                //16：文件系统升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','文件系统升级失败，重启或关闭设备前务必保证文件系统升级成功','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','File system upgrade failed. Make sure the file system is upgraded successfully before restarting or switching off the device','Prompt');
		}
		
	}
	else if(remind== 17)               //17：内核升级成功，触发升级
	{
		if(0==language_switch)
		{
			alarm_tip_update_success(2,'确定','取消','内核升级成功，是否重启','温馨提示');
		}
		else
		{
			alarm_tip_update_success(2,'OK','Cancel','Core upgraded successfully. Do you want to restart?','Prompt');
		}
		
	}
	else if(remind== 18)              //18：内核升级失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','内核升级失败，重启或关闭设备前务必保证内核升级成功','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Core upgrade failed. Make sure the core is upgraded successfully before restarting or switching off the device','Prompt');
		}
		
	}
	else if(remind== 19)              //19：保存用户信息成功
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','保存信息成功，重新登录生效','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Saved successfully. Please sign in again','Prompt');
		}
		
	}
	else if(remind== 20)             //20：保存用户信息失败
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','保存信息失败','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Failed to save information','Prompt');
		}
		
	}
	else if(remind== 21)               //21：用户模式读取操作之前先校验用户信息，通过触发真正读取操作
	{
		readParam();
	}
	else if(remind== 22)               //22：用户模式操作之前先校验用户信息，校验失败
	{
		if(0==language_switch)
		{
			alarm_tip_user_log(1,'确定','取消','用户信息有误，请重新登录','温馨提示');
		}
		else
		{
			alarm_tip_user_log(1,'OK','Cancel','Incorrect user information. Please sign in again','Prompt');
		}
		
	}
	else if(remind== 23)            //23：用户模式写入操作之前先校验用户信息，通过触发真正写入操作
	{
		writeParam();
	}
	else if(remind== 24)           //24：用户模式重启操作之前先校验用户信息，通过触发真正重启操作
	{
		reboot();
	}
	else if(remind== 25)            //25：用户模式保存用户信息操作之前先校验用户信息，通过触发真正保存操作
	{
		set_code();
	}
	else if(remind== 26)         //26：当前操作之前，设备正处于设备树升级状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行设备树升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the device tree. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
	}
	else if(remind== 27)          //27：当前操作之前，设备正处于FPGA程序升级状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行FPGA程序升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the FPGA program. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
		
	}
	else if(remind== 28)          //28：当前操作之前，设备正处于BOOT升级状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行BOOT升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the BOOT. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
		
	}
	else if(remind== 29)          //29：当前操作之前，设备正处于应用程序升级状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行应用程序升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the applications. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
	}
	else if(remind== 30)          //30：当前操作之前，设备正处于文件系统升级状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行文件系统升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the file system. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
		
	}
	else if(remind== 31)             //31：当前操作之前，设备正处于内核升级状态，操作互斥
	{ 
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行内核升级操作，请勿关闭设备、关闭网页，保证设备与电脑网络通畅','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','Another user is upgrading the core. Do not switch off the device or close the webpage, and keep connection between device and computer','Prompt');
		}
	}
	else if(remind== 32)           //32：当前操作之前，设备正处于读取状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行读取操作，请等待','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Another user is reading data. Please wait......','Prompt');
		}
		
	}
	else if(remind== 33)         //33：当前操作之前，设备正处于写入状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行写入操作，请等待','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Another user is writing data. Please wait......','Prompt');
		}
		
	}
	else if(remind== 34)           //34：当前操作之前，设备正处于重启状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行重启操作，请等待','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Another user is restarting the system. Please wait......','Prompt');
		}
	}
	else if(remind== 35)          //35：当前操作之前，设备正处于保存用户信息状态，操作互斥
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','其它用户正在进行保存用户信息操作，请等待','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Another user is saving the user information. Please wait......','Prompt');
		}
	}
	else if(remind== 36)             //36：点击重启，由于设备树--基础文件升级失败，无法重启
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前设备树升级失败，请重新升级后重启','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Device tree upgrade failed. Please upgrade again and then restart the device','Prompt');
		}
		
	}
	else if(remind== 37)            //37：点击重启，由于BOOT--基础文件升级失败，无法重启
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前BOOT升级失败，请重新升级后重启','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Boot upgrade failed. Please upgrade again and then restart the device','Prompt');
		}
	}
	else if(remind== 38)           //38：点击重启，由于FPGA程序升级失败，无法重启（应用程序重启之后会自动加载rbf，该提示可以去掉）
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前FPGA程序升级失败，请重新升级后重启','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','FPGA program upgrade failed. Please upgrade again and then restart the device','Prompt');
		}
		
	}
	else if(remind== 39)          //39：点击重启，由于文件系统--基础文件升级失败，无法重启
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前文件系统升级失败，请重新升级后重启','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','File system upgrade failed. Please upgrade again and then restart the device','Prompt');
		}
		
	}
	else if(remind== 40)        //40：点击重启，由于内核--基础文件升级失败，无法重启
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','当前内核升级失败，请重新升级后重启','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Core upgrade failed. Please upgrade again and then restart the device','Prompt');
		}
		
	}
	else if(remind== 41)        //41：界面保存频段标志与设备不一致
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','频段不一致，请重新配置','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The frequency band does not match. Please make correct configuration','Prompt');
		}
		
	}
	else if(remind== 42)        //40：界面保存频段标志与设备不一致，并且设备支持频段信息有误
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		}
		else
		{
			alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		}
	}
}





/**
 * @Delete_validate_Form    提交表单之前让validationEngine自动验证表单控件失效
 * @author		wdz
 */
function Delete_validate_Form(){
	$("#form1").validationEngine("detach",{
			scroll : false,
			//binded:false,
			showArrow : false,
			promptPosition : "inline",
			maxErrorsPerField : 1,
			showOneMessage : true,
			addPromptClass : " formError-text",
			autoHidePrompt : true,
			autoHideDelay : 2000,
			autoPositionUpdate: true,
		});
}


////////////////////////////升级////////////////////////////////////////////

/**
 * @select_file    文件升级
 * @author		wdz
 */
function select_file()
{
	var tmp_name=document.getElementById("upfile").value;  
	var language_switch=$("#language_switch").val();	
	if(tmp_name == "")
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','升级路径不能为空','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','Path cannot be empty','Prompt');
		}
	}
	else
	{
		var selectedFile = document.getElementById("path").files[0];//获取读取的File对象
        var name = selectedFile.name;//读取选中文件的文件名
		//var size = selectedFile.size;//读取选中文件的大小
		//console.log("文件名:"+name+"大小："+size);
		convert_data_to_def();
		if(name == "soc_system_dtb.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_dtb");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_dtb");
			}
			 
		}
		else  if(name =="bootloader.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_uboot");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_uboot");
			}
		}
		else  if(name == "ramdisk_image.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_file");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_file");
			}
			
		}
		else  if(name == "loadapp.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_loadapp");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_loadapp");
			}
			
		}
		else  if(name == "soc_system_rbf.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_rbf");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_rbf");
			}
		}
		else  if(name == "zImage.bin")
		{
			if(0==language_switch)
			{
				alarm_tip_update__return(2,'确定','取消',"升级之前请确保设备电量充足，是否升级",'温馨提示',"transmit_zimage");
			}
			else
			{
				alarm_tip_update__return(2,'OK','Cancel',"Make sure the device battery is high. Do you want to continue?",'Prompt',"transmit_zimage");
			}
			
		}
		else
		{
			if(0==language_switch)
			{
				alarm_tip(1,'确定','取消','升级文件格式错误','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The format of upgrade file is wrong','Prompt');
			}
			
		}
	}
    
}

/**
 * @alarm_tip_update__return    升级重启提示框，点击“确定”触发升级操作
 * @author		wdz
 */
function alarm_tip_update__return(btns_num,btn1,btn2,option_info,option_title,key){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, yes : function(index){ layer.close(index);document.getElementById("tmp_team").value=$("#teamIdNow").val();
																						document.getElementById("tmp_freq").value=$("#frePointNow").val();
																						var language_switch=$("#language_switch").val();
																						document.getElementById(key).click(); 
																						if("transmit_dtb" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('设备树升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('Device tree upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						else if("transmit_uboot" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('BOOT升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('BOOT upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						else if("transmit_loadapp" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('应用程序升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('Applications upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						else if("transmit_file" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('文件系统升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('File system upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						else if("transmit_rbf" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('FPGA程序升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('FPGA program upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						else if("transmit_zimage" == key)
																						{
																							if(0==language_switch)
																							{
																								layer.load('内核升级中，请勿关闭设备、关闭网页，保证设备与电脑网络通畅');
																							}
																							else
																							{
																								layer.load('Core upgrade in progress. Do not switch off the device or close the webpage, and keep connection between device and computer');
																							}
																						}
																						
																					}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}

/**
 * @path_click    浏览--选择文件对话框
 * @author		wdz
 */
function path_click()
{
	$("#path").click();
}

/**
 * @file_path    升级文件文本框显示文件名
 * @author		wdz
 */
function file_path()
{
	var name = document.getElementById("path").value;
	var array=name.split("\\");
	document.getElementById("upfile").value=array[array.length-1];
}


/**
 * @validate_linkIp    校验IP
 * @author		wdz
 */
function validate_linkIp()
{
	var linkIP = document.getElementById('linkIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.]((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){2}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(linkIP);
}

/**
 * @validate_linkIp_add    校验IP(例 192.168.077.1非法格式)
 * @author		wdz
 */
function validate_linkIp_add()
{
	var linkIP = document.getElementById('linkIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.](((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))[.]){2}((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(linkIP);
}

/**
 * @validate_mask    校验子网掩码
 * @author		wdz
 */
function validate_mask()
{
	var mask = document.getElementById('mask').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(mask);
}


/**
 * @validate_mask_add   校验子网掩码(例 192.168.077.1非法格式)
 * @author		wdz
 */
function validate_mask_add()
{
	var mask = document.getElementById('mask').value;
	var ret=/^(((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))[.]){3}((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(mask);
}


/**
 * @validate_gateWay   校验网关
 * @author		wdz
 */
function validate_gateWay()
{
	var gateWay = document.getElementById('gateWay').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.]((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){2}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(gateWay);
}

/**
 * @validate_gateWay_add    校验网管(例 192.168.077.1非法格式)
 * @author		wdz
 */
function validate_gateWay_add()
{
	var gateWay = document.getElementById('gateWay').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.](((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))[.]){2}((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(gateWay);
}
/**
 * @validate_gateWay_value    校验gateWay是否与IP在同一网段
 * @author		wdz
 */
function validate_gateWay_value()
{
	var linkIP = document.getElementById('linkIP').value;
	var mask = document.getElementById('mask').value;
	var gateWay = document.getElementById('gateWay').value;
	
	var linkIP_num = 0;
	var linkIP_aNum = linkIP.split(".");
	
	linkIP_num += parseInt(linkIP_aNum[0]) << 24;  
    linkIP_num += parseInt(linkIP_aNum[1]) << 16;  
    linkIP_num += parseInt(linkIP_aNum[2]) << 8;  
    linkIP_num += parseInt(linkIP_aNum[3]) << 0;  
    linkIP_num = linkIP_num >>> 0;//这个很关键，不然可能会出现负数的情况  		
	
	var mask_num = 0;
	var mask_aNum = mask.split(".");
	
	mask_num += parseInt(mask_aNum[0]) << 24;  
    mask_num += parseInt(mask_aNum[1]) << 16;  
    mask_num += parseInt(mask_aNum[2]) << 8;  
    mask_num += parseInt(mask_aNum[3]) << 0;  
    mask_num = mask_num >>> 0;//这个很关键，不然可能会出现负数的情况 
	
	var gateWay_num = 0;
	var gateWay_aNum = gateWay.split(".");
	
	gateWay_num += parseInt(gateWay_aNum[0]) << 24;  
    gateWay_num += parseInt(gateWay_aNum[1]) << 16;  
    gateWay_num += parseInt(gateWay_aNum[2]) << 8;  
    gateWay_num += parseInt(gateWay_aNum[3]) << 0;  
    gateWay_num = gateWay_num >>> 0;//这个很关键，不然可能会出现负数的情况
	
	var linkIP_and_mask=linkIP_num&mask_num;
	var gateWay_and_mask=gateWay_num&mask_num;
	
	if(linkIP_and_mask != gateWay_and_mask)
	{
		return 0;
	}
	return 1;
	
}

/**
 * @validate_centerIP   校验中心IP
 * @author		wdz
 */
function validate_centerIP()
{
	var centerIP = document.getElementById('centerIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.]((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){2}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(centerIP);
}

/**
 * @validate_centerIP_add   校验中心IP包含多余零
 * @author		wdz
 */
function validate_centerIP_add()
{
	var centerIP = document.getElementById('centerIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.](((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))[.]){2}((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(centerIP);
}


/**
 * @validate_radioIP   校验中转台IP
 * @author		wdz
 */
function validate_radioIP()
{
	var radioIP = document.getElementById('radioIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.]((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){2}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(radioIP);
}

/**
 * @validate_centerIP_add   校验中转台IP包含多余零
 * @author		wdz
 */
function validate_radioIP_add()
{
	var radioIP = document.getElementById('radioIP').value;
	var ret=/^([1-9]\d?|1\d{2}|2[01]\d|22[0-3])[.](((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))[.]){2}((0)|(1[0-9]{0,2})|([2-9][0-9]{0,1})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(radioIP);
}
/**
 * @validate_centerID   校验互联ID
 * @author		wdz
 */
function validate_centerID()
{
	var centerID = document.getElementById('centerID').value;
	var ret=/^\d+$/;
	if(ret.test(centerID)== 0)
	{
		return 0;
	}
	else
	{
		if((centerID < 1)||(centerID > 51200))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}



/**
 * @validate_epkID  校验本机编号
 * @author		wdz
 */
function validate_epkID()
{
	var epkID = document.getElementById('epkID').value;
	var ret=/^\d+$/;
	if(ret.test(epkID)== 0)
	{
		return 0;
	}
	else
	{
		if((epkID < 1)||(epkID > 32))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_intercomID  校验对讲ID
 * @author		wdz
 */
function validate_intercomID()
{
	var intercomID = document.getElementById('intercomID').value;
	var ret=/^\d+$/;
	if(ret.test(intercomID)== 0)
	{
		return 0;
	}
	else
	{
		if((intercomID < 1)||(intercomID > 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_epkCode  校验链路色码
 * @author		wdz
 */
function validate_epkCode()
{
	var epkCode = document.getElementById('epkCode').value;
	var ret=/^\d+$/;
	if(ret.test(epkCode)== 0)
	{
		return 0;
	}
	else
	{
		if((epkCode < 0)||(epkCode > 15))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_termCode  校验终端色码
 * @author		wdz
 */
function validate_termCode()
{
	var termCode = document.getElementById('termCode').value;
	var ret=/^\d+$/;
	if(ret.test(termCode)== 0)
	{
		return 0;
	}
	else
	{
		if((termCode < 0)||(termCode> 15))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/**
 * @validate_devOverTime  校验链路业务保持时间
 * @author		wdz
 */
function validate_devOverTime()
{
	var devOverTime = document.getElementById('devOverTime').value;
	var ret=/^\d+$/;
	if(ret.test(devOverTime)== 0)
	{
		return 0;
	}
	else
	{
		if((devOverTime < 1)||(devOverTime> 8))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
/**
 * @validate_termGpsOverTime  校验终端业务保持时间
 * @author		wdz
 */
function validate_termGpsOverTime()
{
	var termGpsOverTime = document.getElementById('termGpsOverTime').value;
	var ret=/^\d+$/;
	if(ret.test(termGpsOverTime)== 0)
	{
		return 0;
	}
	else
	{
		if((termGpsOverTime < 5)||(termGpsOverTime> 16))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
/*
///////验证终端遥晕激活超时时间///////////
function validate_termDizzyOverTime()
{
	var termDizzyOverTime = document.getElementById('termDizzyOverTime').value;
	var ret=/^\d+$/;
	if(ret.test(termDizzyOverTime)== 0)
	{
		return 0;
	}
	else
	{
		if((termDizzyOverTime < 3)||(termDizzyOverTime> 10))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

*/


/**
 * @validate_devCallTimeout  校验通话限时
 * @author		wdz
 */
///////验证通话限时///////////
function validate_devCallTimeout()
{
	var devCallTimeout = document.getElementById('devCallTimeout').value;
	var ret=/^\d+$/;
	if(ret.test(devCallTimeout)== 0)
	{
		return 0;
	}
	else
	{
		if((devCallTimeout < 20)||(devCallTimeout> 500))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
/*
///////验证邻点周期///////////
function validate_neighbor_period()
{
	var neighbor_period = document.getElementById('neighbor_period').value;
	var ret=/^\d*[02468]$/;
	if(ret.test(neighbor_period)== 0)
	{
		return 0;
	}
	else
	{
		if((neighbor_period < 2)||(neighbor_period> 20))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
*/
/*
///////验证关闭发射场强门限///////////
function validate_close_transmit_threshold()
{
	var close_transmit_threshold = document.getElementById('close_transmit_threshold').value;
	var ret=/^\d+$/;
	if(ret.test(close_transmit_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((close_transmit_threshold < 0)||(close_transmit_threshold> 700))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

///////验证恢复发射场强门限///////////
function validate_resume_transmit_threshold()
{
	var resume_transmit_threshold = document.getElementById('resume_transmit_threshold').value;
	var ret=/^\d+$/;
	if(ret.test(resume_transmit_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((resume_transmit_threshold < 0)||(resume_transmit_threshold> 65535))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
*/


/**
 * @validate_tempratue_alarm_start_threshold  校验温度告警触发门限
 * @author		wdz
 */
///////验证温度告警触发门限///////////
function validate_tempratue_alarm_start_threshold()
{
	var tempratue_alarm_start_threshold = document.getElementById('tempratue_alarm_start_threshold').value;
	var ret=/^\d+$/;
	if(ret.test(tempratue_alarm_start_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((tempratue_alarm_start_threshold < 0)||(tempratue_alarm_start_threshold> 100))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/**
 * @validate_tempratue_alarm_close_threshold  校验温度告警关闭门限
 * @author		wdz
 */
///////验证温度告警关闭门限///////////
function validate_tempratue_alarm_close_threshold()
{
	var tempratue_alarm_close_threshold = document.getElementById('tempratue_alarm_close_threshold').value;
	var ret=/^\d+$/;
	if(ret.test(tempratue_alarm_close_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((tempratue_alarm_close_threshold < 0)||(tempratue_alarm_close_threshold> 100))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_alarm_count  校验人员告警上报次数
 * @author		wdz
 */
///////验证人员告警上报次数///////////
function validate_alarm_count()
{
	var alarm_count = document.getElementById('alarm_count').value;
	var ret=/^\d+$/;
	if(ret.test(alarm_count)== 0)
	{
		return 0;
	}
	else
	{
		if((alarm_count < 0)||(alarm_count> 255))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/**
 * @validate_close_transmit_rssi_threshold  校验关闭发射场强门限
 * @author		wdz
 */
///////验证关闭发射场强门限//////////////////////
function validate_close_transmit_rssi_threshold()
{
	var close_transmit_rssi_threshold = document.getElementById('close_transmit_rssi_threshold').value;
	var ret=/^\-\d+$/;
	if(ret.test(close_transmit_rssi_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((close_transmit_rssi_threshold < -80)||(close_transmit_rssi_threshold> -40))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_resume_transmit_rssi_threshold  校验恢复发射场强门限
 * @author		wdz
 */
///////验证恢复发射场强门限//////////////////////
function validate_resume_transmit_rssi_threshold()
{
	var resume_transmit_rssi_threshold = document.getElementById('resume_transmit_rssi_threshold').value;
	var ret=/^\-\d+$/;
	if(ret.test(resume_transmit_rssi_threshold)== 0)
	{
		return 0;
	}
	else
	{
		if((resume_transmit_rssi_threshold <-95)||(resume_transmit_rssi_threshold> -65))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/*

///////验证当前组ID///////////
function validate_teamIdNow()
{
	var teamIdNow = document.getElementById('teamIdNow').value;
	var ret=/^\d+$/;
	if(ret.test(teamIdNow)== 0)
	{
		return 0;
	}
	else
	{
		if((teamIdNow < 1)||(teamIdNow> 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}



///////验证当前频点///////////
function validate_frePointNow()
{
	var frePointNow = document.getElementById('frePointNow').value;
	var ret=/^\d+$/;;
	if(ret.test(frePointNow)== 0)
	{
		return 0;
	}
	else
	{
		if((frePointNow < 350000000)||(frePointNow> 400000000))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
*/

/**
 * @validate_team_item  校验组呼号
 * @author		wdz
 */
///////验证组///////////
function validate_team_item(team_num)
{
	var team1 = document.getElementById(team_num).value;
	var ret=/^[1-9]\d*$/;
	if(ret.test(team1)== 0)
	{
		return 0;
	}
	else
	{
		if((team1 < 1)||(team1> 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @alarm_tip_groupNumber  组呼号不连续，清空提示框
 * @author		wdz
 */
function alarm_tip_groupNumber(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_groupNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var team_string='team';
													team_string=team_string+num;
													document.getElementById(team_string).value=""
													document.getElementById(team_string).disabled=true;
													document.getElementById(team_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var team_string='team';
													team_string=team_string+j;
													document.getElementById(team_string).readOnly=false;
												}
												var team_string='team'+document.getElementById('invalidate_groupNumber').value;
												if(document.getElementById(team_string).value=="")
												{
													if(document.getElementById('invalidate_groupNumber').value==1)
													{
														document.getElementById('team1').disabled=false;
														document.getElementById('team1').focus();
													}
													else
													{
														var team_current_value=document.getElementById("tmp_team").value;
														add_id_option();
														if(team_current_value !="")
														{
															$("#teamIdNow option").each(function()
															{
																var tmp_value = $(this).val();
																if(parseInt(tmp_value) == parseInt(team_current_value))
																{
																	document.getElementById("teamIdNow").value=team_current_value;
																}
															})
														}
														num=document.getElementById('invalidate_groupNumber').value;
														num=parseInt(num);
														for(var i=1;i<num+1;i++)
														{
															var team_string='team';
															team_string=team_string+i;
															document.getElementById(team_string).disabled=false;
														}
													}
												}
												else
												{
													document.getElementById(team_string).disabled=false;
													document.getElementById(team_string).focus();
												}
												
													
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_groupNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var team_string='team';
													team_string=team_string+num;
													if(validate_gsmComm_item(team_string)==0)
													{
														document.getElementById(team_string).disabled=false;
														break;
													}	
													else
													{
														document.getElementById(team_string).disabled=false;
													}	
												}
												for(var j=1;j<11;j++)
												{
													var team_string='team';
													team_string=team_string+j;
													document.getElementById(team_string).readOnly=false;
												}
												num= document.getElementById('invalidate_groupNumber').value;
												var team_string='team';
												team_string=team_string+num;
												document.getElementById(team_string).disabled=false;
												document.getElementById(team_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_groupNumber_en  组呼号不连续，清空提示框（英文）
 * @author		wdz
 */
function alarm_tip_groupNumber_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_groupNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var team_string='team';
													team_string=team_string+num;
													document.getElementById(team_string).value=""
													document.getElementById(team_string).disabled=true;
													document.getElementById(team_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var team_string='team';
													team_string=team_string+j;
													document.getElementById(team_string).readOnly=false;
												}
												var team_string='team'+document.getElementById('invalidate_groupNumber').value;
												if(document.getElementById(team_string).value=="")
												{
													if(document.getElementById('invalidate_groupNumber').value==1)
													{
														document.getElementById('team1').disabled=false;
														document.getElementById('team1').focus();
													}
													else
													{
														var team_current_value=document.getElementById("tmp_team").value;
														add_id_option();
														if(team_current_value !="")
														{
															$("#teamIdNow option").each(function()
															{
																var tmp_value = $(this).val();
																if(parseInt(tmp_value) == parseInt(team_current_value))
																{
																	document.getElementById("teamIdNow").value=team_current_value;
																}
															})
														}
														num=document.getElementById('invalidate_groupNumber').value;
														num=parseInt(num);
														for(var i=1;i<num+1;i++)
														{
															var team_string='team';
															team_string=team_string+i;
															document.getElementById(team_string).disabled=false;
														}
													}
												}
												else
												{
													document.getElementById(team_string).disabled=false;
													document.getElementById(team_string).focus();
												}
												
													
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_groupNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var team_string='team';
													team_string=team_string+num;
													if(validate_gsmComm_item(team_string)==0)
													{
														document.getElementById(team_string).disabled=false;
														break;
													}	
													else
													{
														document.getElementById(team_string).disabled=false;
													}	
												}
												for(var j=1;j<11;j++)
												{
													var team_string='team';
													team_string=team_string+j;
													document.getElementById(team_string).readOnly=false;
												}
												num= document.getElementById('invalidate_groupNumber').value;
												var team_string='team';
												team_string=team_string+num;
												document.getElementById(team_string).disabled=false;
												document.getElementById(team_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}
/**
 * @alarm_tip_freqNumber  频率不连续，清空提示框
 * @author		wdz
 */
function alarm_tip_freqNumber(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_freqNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+num;
													document.getElementById(freq_string).value=""
													document.getElementById(freq_string).disabled=true;
													document.getElementById(freq_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+j;
													document.getElementById(freq_string).readOnly=false;
												}
												var freq_string='frePoint'+document.getElementById('invalidate_freqNumber').value;
												if(document.getElementById(freq_string).value=="")
												{
													if(document.getElementById('invalidate_freqNumber').value==1)
													{
														document.getElementById('frePoint1').disabled=false;
														document.getElementById('frePoint1').focus();
													}
													else
													{
														add_freq_option();
														var freq_current_value=document.getElementById("tmp_freq").value;
														if(""!=freq_current_value)
														{
															$("#frePointNow option").each(function() {
															var tmp_value = $(this).val();
															if(parseFloat(tmp_value) == parseFloat(freq_current_value))
															{
																document.getElementById("frePointNow").value=parseFloat(freq_current_value);
															}
															});
														}
														num= document.getElementById('invalidate_freqNumber').value;
														num=parseInt(num);
														for(var i=1;i<num+1;i++)
														{
															var freq_string='frePoint';
															freq_string=freq_string+i;
															document.getElementById(freq_string).disabled=false;
														}
														
													}
													
												}
												else
												{
													document.getElementById(freq_string).disabled=false;
													document.getElementById(freq_string).focus();
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_freqNumber').value;
												var freq_channel=document.getElementById('freq_channel').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+num;
													if(validate_freq_item(freq_string)==0)
													{
														document.getElementById(freq_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(freq_string).disabled=false;
													}
													
													
												}
												for(var j=1;j<11;j++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+j;
													document.getElementById(freq_string).readOnly=false;
												}
												num= document.getElementById('invalidate_freqNumber').value;
												var freq_string='frePoint';
												freq_string=freq_string+num;
												document.getElementById(freq_string).disabled=false;
												document.getElementById(freq_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_freqNumber_en  频率不连续，清空提示框(英语)
 * @author		wdz
 */
function alarm_tip_freqNumber_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_freqNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+num;
													document.getElementById(freq_string).value=""
													document.getElementById(freq_string).disabled=true;
													document.getElementById(freq_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+j;
													document.getElementById(freq_string).readOnly=false;
												}
												var freq_string='frePoint'+document.getElementById('invalidate_freqNumber').value;
												if(document.getElementById(freq_string).value=="")
												{
													if(document.getElementById('invalidate_freqNumber').value==1)
													{
														document.getElementById('frePoint1').disabled=false;
														document.getElementById('frePoint1').focus();
													}
													else
													{
														add_freq_option();
														var freq_current_value=document.getElementById("tmp_freq").value;
														if(""!=freq_current_value)
														{
															$("#frePointNow option").each(function() {
															var tmp_value = $(this).val();
															if(parseFloat(tmp_value) == parseFloat(freq_current_value))
															{
																document.getElementById("frePointNow").value=parseFloat(freq_current_value);
															}
															});
														}
														num= document.getElementById('invalidate_freqNumber').value;
														num=parseInt(num);
														for(var i=1;i<num+1;i++)
														{
															var freq_string='frePoint';
															freq_string=freq_string+i;
															document.getElementById(freq_string).disabled=false;
														}
														
													}
													
												}
												else
												{
													document.getElementById(freq_string).disabled=false;
													document.getElementById(freq_string).focus();
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_freqNumber').value;
												var freq_channel=document.getElementById('freq_channel').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+num;
													if(validate_freq_item(freq_string)==0)
													{
														document.getElementById(freq_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(freq_string).disabled=false;
													}
													
													
												}
												for(var j=1;j<11;j++)
												{
													var freq_string='frePoint';
													freq_string=freq_string+j;
													document.getElementById(freq_string).readOnly=false;
												}
												num= document.getElementById('invalidate_freqNumber').value;
												var freq_string='frePoint';
												freq_string=freq_string+num;
												document.getElementById(freq_string).disabled=false;
												document.getElementById(freq_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @alarm_tip_blackNumber  黑名单不连续，清空提示框
 * @author		wdz
 */
function alarm_tip_blackNumber(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_blackNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var black_string='blkList';
													black_string=black_string+num;
													document.getElementById(black_string).value=""
													document.getElementById(black_string).disabled=true;
													document.getElementById(black_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var black_string='blkList';
													black_string=black_string+j;
													document.getElementById(black_string).readOnly=false;
												}
												var black_string='blkList'+document.getElementById('invalidate_blackNumber').value;
												if(document.getElementById(black_string).value!="")
												{
													document.getElementById(black_string).disabled=false;
													document.getElementById(black_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_blackNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var black_string='blkList';
														black_string=black_string+i;
														document.getElementById(black_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_blackNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var black_string='blkList';
													black_string=black_string+num;
													if(validate_blklist_item(black_string)==0)
													{
														document.getElementById(black_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(black_string).disabled=false;
													}
													
												}
												for(var j=1;j<11;j++)
												{
													var black_string='blkList';
													black_string=black_string+j;
													document.getElementById(black_string).readOnly=false;
												}
												num= document.getElementById('invalidate_blackNumber').value;
												var black_string='blkList';
												black_string=black_string+num;
												document.getElementById(black_string).disabled=false;
												document.getElementById(black_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}

/**
 * @alarm_tip_blackNumber_en  黑名单不连续，清空提示框(英文)
 * @author		wdz
 */
function alarm_tip_blackNumber_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_blackNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var black_string='blkList';
													black_string=black_string+num;
													document.getElementById(black_string).value=""
													document.getElementById(black_string).disabled=true;
													document.getElementById(black_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var black_string='blkList';
													black_string=black_string+j;
													document.getElementById(black_string).readOnly=false;
												}
												var black_string='blkList'+document.getElementById('invalidate_blackNumber').value;
												if(document.getElementById(black_string).value!="")
												{
													document.getElementById(black_string).disabled=false;
													document.getElementById(black_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_blackNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var black_string='blkList';
														black_string=black_string+i;
														document.getElementById(black_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_blackNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var black_string='blkList';
													black_string=black_string+num;
													if(validate_blklist_item(black_string)==0)
													{
														document.getElementById(black_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(black_string).disabled=false;
													}
													
												}
												for(var j=1;j<11;j++)
												{
													var black_string='blkList';
													black_string=black_string+j;
													document.getElementById(black_string).readOnly=false;
												}
												num= document.getElementById('invalidate_blackNumber').value;
												var black_string='blkList';
												black_string=black_string+num;
												document.getElementById(black_string).disabled=false;
												document.getElementById(black_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}

/**
 * @alarm_tip_whiteNumber  白名单不连续，清空提示框
 * @author		wdz
 */
function alarm_tip_whiteNumber(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_whiteNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var white_string='whtList';
													white_string=white_string+num;
													document.getElementById(white_string).value=""
													document.getElementById(white_string).disabled=true;
													document.getElementById(white_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var white_string='whtList';
													white_string=white_string+j;
													document.getElementById(white_string).readOnly=false;
												}
												var white_string='whtList'+document.getElementById('invalidate_whiteNumber').value;
												if(document.getElementById(white_string).value!="")
												{
													document.getElementById(white_string).disabled=false;
													document.getElementById(white_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_whiteNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var white_string='whtList';
														white_string=white_string+i;
														document.getElementById(white_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_whiteNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var white_string='whtList';
													white_string=white_string+num;
													if(validate_whtList_item(white_string)==0)
													{
														document.getElementById(white_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(white_string).disabled=false;
													}
												}
												for(var j=1;j<11;j++)
												{
													var white_string='whtList';
													white_string=white_string+j;
													document.getElementById(white_string).readOnly=false;
												}
												num= document.getElementById('invalidate_whiteNumber').value;
												var white_string='whtList';
												white_string=white_string+num;
												document.getElementById(white_string).disabled=false;
												document.getElementById(white_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}

/**
 * @alarm_tip_whiteNumber_en  白名单不连续，清空提示框（英语）
 * @author		wdz
 */
function alarm_tip_whiteNumber_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_whiteNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var white_string='whtList';
													white_string=white_string+num;
													document.getElementById(white_string).value=""
													document.getElementById(white_string).disabled=true;
													document.getElementById(white_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var white_string='whtList';
													white_string=white_string+j;
													document.getElementById(white_string).readOnly=false;
												}
												var white_string='whtList'+document.getElementById('invalidate_whiteNumber').value;
												if(document.getElementById(white_string).value!="")
												{
													document.getElementById(white_string).disabled=false;
													document.getElementById(white_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_whiteNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var white_string='whtList';
														white_string=white_string+i;
														document.getElementById(white_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_whiteNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var white_string='whtList';
													white_string=white_string+num;
													if(validate_whtList_item(white_string)==0)
													{
														document.getElementById(white_string).disabled=false;
														break;
													}
													else
													{
														document.getElementById(white_string).disabled=false;
													}
												}
												for(var j=1;j<11;j++)
												{
													var white_string='whtList';
													white_string=white_string+j;
													document.getElementById(white_string).readOnly=false;
												}
												num= document.getElementById('invalidate_whiteNumber').value;
												var white_string='whtList';
												white_string=white_string+num;
												document.getElementById(white_string).disabled=false;
												document.getElementById(white_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['450px', '50%']		
				});
}

/**
 * @alarm_tip_gsmNumber  gsm列表不连续，清空提示框
 * @author		wdz
 */

function alarm_tip_gsmNumber(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_gsmNumber').value;
												
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+num;
													document.getElementById(gsm_string).value=""
													document.getElementById(gsm_string).disabled=true;
													document.getElementById(gsm_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+j;
													document.getElementById(gsm_string).readOnly=false;
												}
												var gsm_string='gsmComm'+document.getElementById('invalidate_gsmNumber').value;
												if(document.getElementById(gsm_string).value!="")
												{
													document.getElementById(gsm_string).disabled=false;
													document.getElementById(gsm_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_gsmNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var gsm_string='gsmComm';
														gsm_string=gsm_string+i;
														document.getElementById(gsm_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_gsmNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+num;
													
													if(validate_gsmComm_item(gsm_string)==0)
													{
														document.getElementById(gsm_string).disabled=false;
									
														break;
													}
													else
													{
														document.getElementById(gsm_string).disabled=false;
													
													}
												}
												for(var j=1;j<11;j++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+j;
													document.getElementById(gsm_string).readOnly=false;
												}
												num= document.getElementById('invalidate_gsmNumber').value;
												var gsm_string='gsmComm';
												gsm_string=gsm_string+num;
												document.getElementById(gsm_string).disabled=false;
												document.getElementById(gsm_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}


/**
 * @alarm_tip_gsmNumber_en  gsm列表不连续，清空提示框(英文)
 * @author		wdz
 */

function alarm_tip_gsmNumber_en(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['460px','180px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info, 
						yes : function(index){ layer.close(index);
												var num= document.getElementById('invalidate_gsmNumber').value;
												
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+num;
													document.getElementById(gsm_string).value=""
													document.getElementById(gsm_string).disabled=true;
													document.getElementById(gsm_string).title="";
												}
												for(var j=1;j<11;j++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+j;
													document.getElementById(gsm_string).readOnly=false;
												}
												var gsm_string='gsmComm'+document.getElementById('invalidate_gsmNumber').value;
												if(document.getElementById(gsm_string).value!="")
												{
													document.getElementById(gsm_string).disabled=false;
													document.getElementById(gsm_string).focus();
												}
												else
												{
													num= document.getElementById('invalidate_gsmNumber').value;
													num=parseInt(num);
													for(var i=1;i<num+1;i++)
													{
														var gsm_string='gsmComm';
														gsm_string=gsm_string+i;
														document.getElementById(gsm_string).disabled=false;
													}
												}
											},
						no:  function(index){ layer.close(index);
												
												var num= document.getElementById('invalidate_gsmNumber').value;
												num=1+parseInt(num);
												for(;num<11;num++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+num;
													
													if(validate_gsmComm_item(gsm_string)==0)
													{
														document.getElementById(gsm_string).disabled=false;
									
														break;
													}
													else
													{
														document.getElementById(gsm_string).disabled=false;
													
													}
												}
												for(var j=1;j<11;j++)
												{
													var gsm_string='gsmComm';
													gsm_string=gsm_string+j;
													document.getElementById(gsm_string).readOnly=false;
												}
												num= document.getElementById('invalidate_gsmNumber').value;
												var gsm_string='gsmComm';
												gsm_string=gsm_string+num;
												document.getElementById(gsm_string).disabled=false;
												document.getElementById(gsm_string).focus();
											}
						 },
				title : [option_title,true],
				offset: ['400px', '50%']		
				});
}

/**
 * @groupNumber_focus  组呼列表获取焦点（disable其它列表，防止其它列表获取焦点  触发事件；title清空，防止由于disable提示不正常显示）
 * @author		wdz
 */
function groupNumber_focus(num)
{
	document.getElementById("read_down").value=0;
	document.getElementById("import_down").value=0;
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var team_string='team';
			team_string=team_string+j;
			
			
			document.getElementById(team_string).disabled=true;
			//	document.getElementById(team_string).style.color="#000000";
			//	document.getElementById(team_string).style.borderColor="#aaa";
			//	document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(team_string).title="";	
			
		}
	}
}


/**
 * @groupNumber_mouse  组呼列表获取鼠标悬停事件（有效组呼号添加title，防止由于disable提示不正常显示）
 * @author		wdz
 */
function groupNumber_mouse(num)
{
	var team_string='team';
	team_string=team_string+num;
	var language_switch=$("#language_switch").val();
	if(document.getElementById(team_string).disabled == false)
	{
		if(0==language_switch)
		{
			if(num ==1)
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415,不能为空";
			}
			else
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1-16776415";
			}
		}
		else
		{
			if(num==1)
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415. It cannot be empty";
			}	
			else
			{
				document.getElementById(team_string).title="Group call number. Range: 1-16776415";
			}
		}
	}
	
}


/**
 * @freqNumber_focus  频率列表获取焦点（disable其它列表，防止其它列表获取焦点  触发事件；title清空，防止由于disable提示不正常显示）
 * @author		wdz
 */
function freqNumber_focus(num)
{
	document.getElementById("read_down").value=0;
	document.getElementById("import_down").value=0;
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var freq_string='frePoint';
			freq_string=freq_string+j;
			
			
			document.getElementById(freq_string).disabled=true;
			//	document.getElementById(team_string).style.color="#000000";
			//	document.getElementById(team_string).style.borderColor="#aaa";
			//	document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(freq_string).title="";		
			
		}
		
	}
}


/**
 * @freqNumber_mouse  频率列表获取鼠标悬停事件（有效频率添加title，防止由于disable提示不正常显示）
 * @author		wdz
 */
function freqNumber_mouse(num)
{
	var freq_string='frePoint';
	freq_string=freq_string+num;
	var freq_channel=document.getElementById('freq_channel').value;
	var language_switch=$("#language_switch").val();
	if(document.getElementById(freq_string).disabled == false)
	{
		if(freq_channel ==1)
		{
			if(0==language_switch)
			{
				if(num ==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350-400；输入格式：最多6位精度位数";
				}
			}
			else
			{
				if(num ==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 350-400. At most 6 decimal places are allowed";
				}
			}
			
		}
		else if(freq_channel == 2)
		{
			if(0==language_switch)
			{
				if(num ==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470,不能为空；输入格式：最多6位精度位数";
				}
				else
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410-470；输入格式：最多6位精度位数";
				}
			}
			else
			{
				if(num==1)
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. It cannot be empty. At most 6 decimal places are allowed";
				}
				else
				{
					document.getElementById(freq_string).title="Frequency used by the device. Range: 410-470. At most 6 decimal places are allowed";
				}
			}
			
		}
	}
}

/**
 * @blackNumber_focus  黑名单列表获取焦点（disable其它列表，防止其它列表获取焦点  触发事件；title清空，防止由于disable提示不正常显示）
 * @author		wdz
 */
function blackNumber_focus(num)
{
	document.getElementById("read_down").value=0;
	document.getElementById("import_down").value=0;
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var black_string='blkList';
			black_string=black_string+j;
			
			
			document.getElementById(black_string).disabled=true;
			//	document.getElementById(team_string).style.color="#000000";
			//	document.getElementById(team_string).style.borderColor="#aaa";
			//	document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(black_string).title="";		
			
		}
		
	}
}
/**
 * @blackNumber_mouse  黑名单列表获取鼠标悬停事件（有效黑名单添加title，防止由于disable提示不正常显示）
 * @author		wdz
 */
function blackNumber_mouse(num)
{
	var black_string='blkList';
	black_string=black_string+num;
	var language_switch=$("#language_switch").val();
	if(document.getElementById(black_string).disabled==false)
	{
		if(0==language_switch)
		{
			document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(black_string).title="The number calls from which will be blocked. Enter at most 30 digits";
		}
	}
}

/**
 * @whiteNumber_focus  白名单列表获取焦点（disable其它列表，防止其它列表获取焦点  触发事件；title清空，防止由于disable提示不正常显示）
 * @author		wdz
 */
function whiteNumber_focus(num)
{
	document.getElementById("read_down").value=0;
	document.getElementById("import_down").value=0;
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var white_string='whtList';
			white_string=white_string+j;
			
			
			document.getElementById(white_string).disabled=true;
			//	document.getElementById(team_string).style.color="#000000";
			//	document.getElementById(team_string).style.borderColor="#aaa";
			//	document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(white_string).title="";		
			
		}
		
	}
}


/**
 * @whiteNumber_mouse  白名单列表获取鼠标悬停事件（有效白名单添加title，防止由于disable提示不正常显示）
 * @author		wdz
 */
function whiteNumber_mouse(num)
{
	var white_string='whtList';
	white_string=white_string+num;
	var language_switch=$("#language_switch").val();
	if(document.getElementById(white_string).disabled==false)
	{
		if(0==language_switch)
		{
			document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		}
		else
		{
			document.getElementById(white_string).title="The number calls from which will be received. Enter at most 30 digits";
		}
	}
}


/**
 * @gsmNumber_focus  gsm列表获取焦点（disable其它列表，防止其它列表获取焦点  触发事件；title清空，防止由于disable提示不正常显示）
 * @author		wdz
 */
function gsmNumber_focus(num)
{
	document.getElementById("read_down").value=0;
	document.getElementById("import_down").value=0;
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var gsm_string='gsmComm';
			gsm_string=gsm_string+j;
			
			
			document.getElementById(gsm_string).disabled=true;
			//	document.getElementById(team_string).style.color="#000000";
			//	document.getElementById(team_string).style.borderColor="#aaa";
			//	document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(gsm_string).title="";		
			
		}
		
	}
}

/**
 * @gsmNumber_mouse  gsm列表获取鼠标悬停事件（有效gsm添加title，防止由于disable提示不正常显示）
 * @author		wdz
 */
function gsmNumber_mouse(num)
{
	var gsm_string='gsmComm';
	gsm_string=gsm_string+num;
	var language_switch=$("#language_switch").val();
	if(document.getElementById(gsm_string).disabled == false)
	{
		if(0==language_switch)
		{
			document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
		}
		else
		{
			document.getElementById(gsm_string).title="GSM call number. Enter at most 30 characters. For example, +1234 or 1234";
		}
	}
}


/**
 * @groupNumber_blur  组呼失去焦点（判断连续、重复，更新当前组）
 * @author		wdz
 */
function groupNumber_blur(num)
{
	var team_string='team';
	var i=0;
	var j=0;
	var next=0;
	team_string=team_string+num;
	document.getElementById(team_string).title="";
	var team_current_value=document.getElementById("teamIdNow").value;
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("team_span").focus();
	var language_switch=$("#language_switch").val();
	if(1==document.getElementById("read_down").value)
	{
		document.getElementById("read_down").value=0;
		return ;
	}
	if(1==document.getElementById("import_down").value)
	{
		document.getElementById("import_down").value=0;
		return ;
	}
	for(i=1;i<11;i++)
	{
		team_string='team';
		team_string=team_string+i;
		if(validate_team_item(team_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		team_string='team';
		team_string=team_string+i;
		var base_value=document.getElementById(team_string).value;
		for(j=i+1;j<next;j++)
		{
			team_string='team';
			team_string=team_string+j;
			var current_value=document.getElementById(team_string).value;
			
			if(parseInt(base_value)==parseInt(current_value))
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		team_string='team';
		team_string=team_string+next;
		var current_value=document.getElementById(team_string).value;
		var validate_result=validate_team_item(team_string);
		
		if("" == current_value)
		{
			if(next==1)
			{
				document.getElementById('invalidate_groupNumber').value=next;
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=true;
				}
				$("#teamIdNow").empty();
				if(0==language_switch)
				{
					alarm_tip_groupNumber_return(1,'确定','取消',"组1不能为空",'温馨提示');
				}
				else
				{
					alarm_tip_groupNumber_return(1,'OK','Cancel',"Group 1 cannot be empty",'Prompt');
				}
				return ;
			}
			if(next==10)
			{
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=false;
				}
				add_id_option();
				if(team_current_value !="")
				{
					$("#teamIdNow option").each(function()
					{
						tmp_value = $(this).val();
						if(parseInt(tmp_value) == parseInt(team_current_value))
						{
							document.getElementById("teamIdNow").value=team_current_value;
						}
					})
				}
			
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					team_string='team';
					team_string=team_string+i;
					if(document.getElementById(team_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_groupNumber').value=next;
					for(j=1;j<11;j++)
					{
						team_string='team';
						team_string=team_string+j;
						document.getElementById(team_string).readOnly=true;
					}
					$("#teamIdNow").empty();
				
					team_string="组";
					team_string=team_string+next;
					if(0==language_switch)
					{
						team_string="组";
						team_string=team_string+num;
						alarm_tip_groupNumber(2,'确定','取消',team_string+'输入为空，为保证组呼列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						team_string="Group ";
						team_string=team_string+num;
						alarm_tip_groupNumber(2,'OK','Cancel',"Deleting "+team_string+' deletes all followed groups. Do you want to continue?','Prompt');
					}
					return ;
				}
				else
				{
					add_id_option();
					if(team_current_value !="")
					{
						$("#teamIdNow option").each(function()
						{
							tmp_value = $(this).val();
							if(parseInt(tmp_value) == parseInt(team_current_value))
							{
								document.getElementById("teamIdNow").value=team_current_value;
							}
						})
					}
				}
		
			}	
		}
		else if(0==validate_result)
		{
			if(next==10)
			{
				document.getElementById('invalidate_groupNumber').value=next;
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=true;
				}
				$("#teamIdNow").empty();
				if(0==language_switch)
				{
					team_string="组";
					team_string=team_string+num;
					alarm_tip_groupNumber_return(1,'确定','取消',team_string+"输入格式错误",'温馨提示');
				}
				else
				{
					team_string="Group ";
					team_string=team_string+num;
					alarm_tip_groupNumber_return(1,'OK','Cancel',"The format of "+team_string+" is wrong",'Prompt');
				}
				return ;
			
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					team_string='team';
					team_string=team_string+i;
					if(document.getElementById(team_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_groupNumber').value=next;
					for(j=1;j<11;j++)
					{
						team_string='team';
						team_string=team_string+j;
						document.getElementById(team_string).readOnly=true;
					}
					$("#teamIdNow").empty();
					if(0==language_switch)
					{
						team_string="组";
						team_string=team_string+num;
						alarm_tip_groupNumber(2,'确定','取消',team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						team_string="Group ";
						team_string=team_string+num;
						alarm_tip_groupNumber_en(2,'OK','Cancel',"The format of "+team_string+' is wrong. If you continue, all followed groups will be deleted. Do you want to continue?','Prompt');
					}
					return ;
				}
				else
				{
					document.getElementById('invalidate_groupNumber').value=next;
					for(j=1;j<11;j++)
					{
						team_string='team';
						team_string=team_string+j;
						document.getElementById(team_string).readOnly=true;
					}
					$("#teamIdNow").empty();
					if(0==language_switch)
					{
						team_string="组";
						team_string=team_string+num;
						alarm_tip_groupNumber_return(1,'确定','取消',team_string+"输入格式错误",'温馨提示');
					}
					else
					{
						team_string="Group ";
						team_string=team_string+num;
						alarm_tip_groupNumber_return(1,'OK','Cancel',"The format of "+team_string+' is wrong,','Prompt');
					}
					return ;
				}
			}
		}
		else
		{
			document.getElementById('invalidate_groupNumber').value=next;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			if(0==language_switch)
			{
				team_string="组";
				team_string=team_string+num;
				alarm_tip_groupNumber_return(1,'确定','取消',team_string+"输入值已存在",'温馨提示');
			}
			else
			{
				team_string="Group ";
				team_string=team_string+num;
				alarm_tip_groupNumber_return(1,'OK','Cancel',"The "+team_string+" already exists",'Prompt');
			}
			return ;
		}
		for(i=1;i<next+1;i++)
		{
			team_string='team';
			team_string=team_string+i;		
			document.getElementById(team_string).disabled=false;			
		}
		for(i=1+next;i<11;i++)
		{
			team_string='team';
			team_string=team_string+i;		
			document.getElementById(team_string).disabled=true;			
		}
	}
	else
	{
		add_id_option();
		if(team_current_value !="")
		{
			$("#teamIdNow option").each(function()
			{
				tmp_value = $(this).val();
				if(parseInt(tmp_value) == parseInt(team_current_value))
				{
					document.getElementById("teamIdNow").value=team_current_value;
				}
			})
		}
		
		for(i=1;i<next;i++)
		{
			team_string='team';
			team_string=team_string+i;		
			document.getElementById(team_string).disabled=false;
					
		}
	}
}

/**
 * @freqNumber_blur  频率失去焦点（判断连续、重复，更新当前频率列表）
 * @author		wdz
 */
function freqNumber_blur(num)
{
	var freq_string='frePoint';
	var i=0;
	var j=0;
	var next=0;
	freq_string=freq_string+num;
	document.getElementById(freq_string).title="";
	var freq_current_value=document.getElementById("frePointNow").value;
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("fre_span").focus();
	var language_switch=$("#language_switch").val();
	if(1==document.getElementById("read_down").value)
	{
		document.getElementById("read_down").value=0;
		return ;
	}
	if(1==document.getElementById("import_down").value)
	{
		document.getElementById("import_down").value=0;
		return ;
	}
	for(i=1;i<11;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		if(validate_freq_item(freq_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		var base_value=document.getElementById(freq_string).value;
		for(j=i+1;j<next;j++)
		{
			freq_string='frePoint';
			freq_string=freq_string+j;
			var current_value=document.getElementById(freq_string).value;
			
			if(parseFloat(base_value)==parseFloat(current_value))
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		freq_string='frePoint';
		freq_string=freq_string+next;
		var current_value=document.getElementById(freq_string).value;
		var validate_result=validate_freq_item(freq_string);
		
		if("" == current_value)
		{
			if(next==1)
			{
				document.getElementById('invalidate_freqNumber').value=next;
				for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=true;
				}
				$("#frePointNow").empty();
				if(0==language_switch)
				{
					alarm_tip_freqNumber_return(1,'确定','取消',"频率1不能为空",'温馨提示');
				}
				else
				{
					alarm_tip_freqNumber_return(1,'OK','Cancel',"Frequency 1 cannot be empty",'Prompt');
				}
				return ;
			}
			if(next==10)
			{
				for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=false;
				}
				add_freq_option();
				if(freq_current_value !="")
				{
					$("#frePointNow option").each(function() {
									tmp_value = $(this).val();
									if(parseFloat(tmp_value) == parseFloat(freq_current_value))
									{
										document.getElementById("frePointNow").value=parseFloat(freq_current_value);
									}
					});
				}
			
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					freq_string='frePoint';
					freq_string=freq_string+i;
					if(document.getElementById(freq_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_freqNumber').value=next;
					for(j=1;j<11;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						document.getElementById(freq_string).readOnly=true;
					}
					$("#frePointNow").empty();
				
					if(0==language_switch)
					{
						freq_string="频率";
						freq_string=freq_string+num;
						alarm_tip_freqNumber(2,'确定','取消',freq_string+'输入为空，为保证频率列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						freq_string="Frequency ";
						freq_string=freq_string+num;
						alarm_tip_freqNumber(2,'OK','Cancel',"Deleting "+freq_string+' deletes all followed frequencies. Do you want to continue?','Prompt');
					}
					return ;
				}
				else
				{
					add_freq_option();
					if(freq_current_value !="")
					{
						$("#frePointNow option").each(function() {
												tmp_value = $(this).val();
												if(parseFloat(tmp_value) == parseFloat(freq_current_value))
												{
													document.getElementById("frePointNow").value=parseFloat(freq_current_value);
												}
							});
					}
				}
		
			}	
		}
		else if(0==validate_result)
		{
			if(next==10)
			{
				document.getElementById('invalidate_freqNumber').value=next;
			    for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=true;
				}
				$("#frePointNow").empty();
				if(0==language_switch)
				{
					freq_string="频率";
					freq_string=freq_string+num;
					alarm_tip_freqNumber_return(1,'确定','取消',freq_string+"输入格式错误",'温馨提示');
				}
				else
				{
					freq_string="Frequency ";
					freq_string=freq_string+num;
					alarm_tip_freqNumber_return(1,'OK','Cancel',"The format of "+freq_string+" is wrong",'Prompt');
				}
				return ;
			
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					freq_string='frePoint';
					freq_string=freq_string+i;
					if(document.getElementById(freq_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_freqNumber').value=next;
					for(j=1;j<11;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						document.getElementById(freq_string).readOnly=true;
					}
					$("#frePointNow").empty();
					if(0==language_switch)
					{
						freq_string="频率";
						freq_string=freq_string+num;
						alarm_tip_freqNumber(2,'确定','取消',freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						freq_string="Frequency ";
						freq_string=freq_string+num;
						alarm_tip_freqNumber_en(2,'OK','Cancel',"The format of "+freq_string+" is wrong. If you continue, all followed frequencies will be deleted. Do you want to continue?",'Prompt');
					}
					return ;
				}
				else
				{
					document.getElementById('invalidate_freqNumber').value=next;
					for(j=1;j<11;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						document.getElementById(freq_string).readOnly=true;
					}
					$("#frePointNow").empty();
					if(0==language_switch)
					{
						freq_string="频率";
						freq_string=freq_string+num;
						alarm_tip_freqNumber_return(1,'确定','取消',freq_string+"输入格式错误",'温馨提示');
					}
					else
					{
						freq_string="Frequency ";
						freq_string=freq_string+num;
						alarm_tip_freqNumber_return(1,'OK','Cancel',"The format of "+freq_string+" is wrong",'Prompt');
					}
					return ;
				}
			}
		}
		else
		{
			document.getElementById('invalidate_freqNumber').value=next;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			if(0==language_switch)
			{
				freq_string="频率";
				freq_string=freq_string+num;
				alarm_tip_freqNumber_return(1,'确定','取消',freq_string+"输入值已存在",'温馨提示');
			}
			else
			{
				freq_string="Frequency ";
				freq_string=freq_string+num;
				alarm_tip_freqNumber_return(1,'OK','Cancel',"The "+freq_string+" already exists",'Prompt');
			}
			return ;
		}
		for(i=1;i<next+1;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;		
			document.getElementById(freq_string).disabled=false;			
		}
		for(i=next+1;i<11;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;		
			document.getElementById(freq_string).disabled=true;
		}
	}
	else
	{
		add_freq_option();
		if(freq_current_value !="")
		{
			$("#frePointNow option").each(function() {
					tmp_value = $(this).val();
					if(parseFloat(tmp_value) == parseFloat(freq_current_value))
					{
						document.getElementById("frePointNow").value=parseFloat(freq_current_value);
					}
			});
		}
		
		for(i=1;i<next;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;		
			document.getElementById(freq_string).disabled=false;
					
		}
	}
}



/**
 * @blackNumber_blur  黑名单失去焦点（判断连续、重复）
 * @author		wdz
 */
function blackNumber_blur(num)
{
	var black_string='blkList';
	var i=0;
	var j=0;
	var next=0;
	black_string=black_string+num;
	document.getElementById(black_string).title=""
	document.getElementById("blk_span").focus();
	var language_switch=$("#language_switch").val();
	if(1==document.getElementById("read_down").value)
	{
		document.getElementById("read_down").value=0;
		return ;
	}
	if(1==document.getElementById("import_down").value)
	{
		document.getElementById("import_down").value=0;
		return ;
	}
	
	for(i=1;i<11;i++)
	{
		black_string='blkList';
		black_string=black_string+i;
		if(validate_blklist_item(black_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		black_string='blkList';
		black_string=black_string+i;
		var base_value=document.getElementById(black_string).value;
		for(j=i+1;j<next;j++)
		{
			black_string='blkList';
			black_string=black_string+j;
			var current_value=document.getElementById(black_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	
	if(next !=11)  //存在无效、空  或者  重复项
	{
		black_string='blkList';
		black_string=black_string+next;
		var current_value=document.getElementById(black_string).value;
		var validate_result=validate_blklist_item(black_string);
		
		if("" == current_value)
		{
			if(next==10)
			{
				for(j=1;j<11;j++)
				{
					black_string='blkList';
					black_string=black_string+j;
					document.getElementById(black_string).readOnly=false;
				}
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					black_string='blkList';
					black_string=black_string+i;
					if(document.getElementById(black_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_blackNumber').value=next;
					for(j=1;j<11;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						document.getElementById(black_string).readOnly=true;
					}
					if(0==language_switch)
					{
						black_string="黑名单";
						black_string=black_string+num;
						alarm_tip_blackNumber(2,'确定','取消',black_string+'输入为空，为保证黑名单列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						black_string="Blacklist ";
						black_string=black_string+num;
						alarm_tip_blackNumber(2,'OK','Cancel',"Deleting "+black_string+' deletes all followed blacklists. Do you want to continue?','Prompt');
					}
					return ;
				}
			}
		}
		else if(0==validate_result)
		{
			if(num==10)
			{
				document.getElementById('invalidate_blackNumber').value=next;
				for(j=1;j<11;j++)
				{
					black_string='blkList';
					black_string=black_string+j;
					document.getElementById(black_string).readOnly=true;
				}
				if(0==language_switch)
				{
					black_string="黑名单";
					black_string=black_string+num;
					alarm_tip_blackNumber_return(1,'确定','取消',black_string+"输入格式错误",'温馨提示');
				}
				else
				{
					black_string="Blacklist ";
					black_string=black_string+num;
					alarm_tip_blackNumber_return(1,'OK','Cancel',"The format of "+black_string+" is wrong",'Prompt');
				}
				return;
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					black_string='blkList';
					black_string=black_string+i;
					if(document.getElementById(black_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_blackNumber').value=next;
					for(j=1;j<11;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						document.getElementById(black_string).readOnly=true;
					}
					if(0==language_switch)
					{
						black_string="黑名单";
						black_string=black_string+num;
						alarm_tip_blackNumber(2,'确定','取消',black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						black_string="Blacklist ";
						black_string=black_string+num;
						alarm_tip_blackNumber_en(2,'OK','Cancel',"The format of "+black_string+" is wrong. If you continue, all followed blacklists will be deleted. Do you want to continue?",'Prompt');
					}
					return ;
				}
				else
				{
					document.getElementById('invalidate_blackNumber').value=next;
					for(j=1;j<11;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						document.getElementById(black_string).readOnly=true;
					}
					if(0==language_switch)
					{
						black_string="黑名单";
						black_string=black_string+num;
						alarm_tip_blackNumber_return(1,'确定','取消',black_string+"输入格式错误",'温馨提示');
					}
					else
					{
						black_string="Blacklist ";
						black_string=black_string+num;
						alarm_tip_blackNumber_return(1,'OK','Cancel',"The format of "+black_string+" is wrong",'Prompt');
					}
					return ;
				}
			}
		}
		else
		{
			document.getElementById('invalidate_blackNumber').value=next;
			for(j=1;j<11;j++)
			{
				black_string='blkList';
				black_string=black_string+j;
				document.getElementById(black_string).readOnly=true;
			}
			if(0==language_switch)
			{
				black_string="黑名单";
				black_string=black_string+num;
				alarm_tip_blackNumber_return(1,'确定','取消',black_string+"输入值已存在",'温馨提示');
			}
			else
			{
				black_string="Blacklist ";
				black_string=black_string+num;
				alarm_tip_blackNumber_return(1,'OK','Cancel',"The "+black_string+" already exists",'Prompt');
			}
			return ;
		}
		for(i=1;i<next+1;i++)
		{
			black_string='blkList';
			black_string=black_string+i;		
			document.getElementById(black_string).disabled=false;			
		}
		for(i=next+1;i<11;i++)
		{
			black_string='blkList';
			black_string=black_string+i;		
			document.getElementById(black_string).disabled=true;
		}
	}
	else
	{
		for(i=1;i<next;i++)
		{
			black_string='blkList';
			black_string=black_string+i;		
			document.getElementById(black_string).disabled=false;			
		}
	}
}



/**
 * @whiteNumber_blur  白名单失去焦点（判断连续、重复）
 * @author		wdz
 */
function whiteNumber_blur(num)
{
	var white_string='whtList';
	var i=0;
	var j=0;
	var next=0;
	white_string=white_string+num;
	document.getElementById(white_string).title="";
	document.getElementById("wht_span").focus();
	var language_switch=$("#language_switch").val();
	if(1==document.getElementById("read_down").value)
	{
		document.getElementById("read_down").value=0;
		return ;
	}
	if(1==document.getElementById("import_down").value)
	{
		document.getElementById("import_down").value=0;
		return ;
	}
	
	for(i=1;i<11;i++)
	{
		white_string='whtList';
		white_string=white_string+i;
		if(validate_whtList_item(white_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		white_string='whtList';
		white_string=white_string+i;
		var base_value=document.getElementById(white_string).value;
		for(j=i+1;j<next;j++)
		{
			white_string='whtList';
			white_string=white_string+j;
			var current_value=document.getElementById(white_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		white_string='whtList';
		white_string=white_string+next;
		var current_value=document.getElementById(white_string).value;
		var validate_result=validate_whtList_item(white_string);
		
		if(""==current_value)
		{
			if(next==10)
			{
				for(j=1;j<11;j++)
				{
					white_string='whtList';
					white_string=white_string+j;
					document.getElementById(white_string).readOnly=false;
				}
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					white_string='whtList';
					white_string=white_string+i;
					if(document.getElementById(white_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_whiteNumber').value=next;
					for(j=1;j<11;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						document.getElementById(white_string).readOnly=true;
					}
					if(0==language_switch)
					{
						white_string="白名单";
						white_string=white_string+num;
						alarm_tip_whiteNumber(2,'确定','取消',white_string+'输入为空，为保证白名单列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						white_string="Whitelist ";
						white_string=white_string+num;
						alarm_tip_whiteNumber(2,'OK','Cancel',"Deleting "+white_string+' deletes all followed whitelists. Do you want to continue?','Prompt');
					}
					return ;
				}
			}
		}
		else if(0==validate_result)
		{
			if(next==10)
			{
				document.getElementById('invalidate_whiteNumber').value=next;
				for(j=1;j<11;j++)
				{
					white_string='whtList';
					white_string=white_string+j;
					document.getElementById(white_string).readOnly=true;
				}
				if(0==language_switch)
				{
					white_string="白名单";
					white_string=white_string+num;
					alarm_tip_whiteNumber_return(1,'确定','取消',white_string+"输入格式错误",'温馨提示');
				}	
				else
				{
					white_string="Whitelist ";
					white_string=white_string+num;
					alarm_tip_whiteNumber_return(1,'OK','Cancel',"The format of "+white_string+" is wrong",'Prompt');
				}
				return;
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					white_string='whtList';
					white_string=white_string+i;
					if(document.getElementById(white_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_whiteNumber').value=next;
					for(j=1;j<11;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						document.getElementById(white_string).readOnly=true;
					}
					if(0==language_switch)
					{
						white_string="白名单";
						white_string=white_string+num;
						alarm_tip_whiteNumber(2,'确定','取消',white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						white_string="Whitelist ";
						white_string=white_string+num;
						alarm_tip_whiteNumber_en(2,'OK','Cancel',"The format of "+white_string+' is wrong. If you continue, all followed whitelists will be deleted. Do you want to continue?','Prompt');
					}
					return ;
				}
				else
				{
					document.getElementById('invalidate_whiteNumber').value=next;
					for(j=1;j<11;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						document.getElementById(white_string).readOnly=true;
					}	
					if(0==language_switch)
					{
						white_string="白名单";
						white_string=white_string+num;
						alarm_tip_whiteNumber_return(1,'确定','取消',white_string+"输入格式错误",'温馨提示');
					}
					else
					{
						white_string="whtList ";
						white_string=white_string+num;
						alarm_tip_whiteNumber_return(1,'OK','Cancel',"The format of "+white_string+' is wrong','Prompt');
					}
					return;
				}
			}
		}
		else
		{
			document.getElementById('invalidate_whiteNumber').value=next;
			for(j=1;j<11;j++)
			{
				white_string='whtList';
				white_string=white_string+j;
				document.getElementById(white_string).readOnly=true;
			}
			if(0==language_switch)
			{
				white_string="白名单";
				white_string=white_string+num;
				alarm_tip_whiteNumber_return(1,'确定','取消',white_string+"输入值已存在",'温馨提示');
			}
			else
			{
				white_string="whtList ";
				white_string=white_string+num;
				alarm_tip_whiteNumber_return(1,'OK','Cancel',"The "+white_string+" already exists",'Prompt');
			}
			return ;
		}
		for(i=1;i<next+1;i++)
		{
			white_string='whtList';
			white_string=white_string+i;		
			document.getElementById(white_string).disabled=false;			
		}
		for(i=next+1;i<11;i++)
		{
			white_string='whtList';
			white_string=white_string+i;		
			document.getElementById(white_string).disabled=true;
		}
	}
	else
	{
		for(i=1;i<next;i++)
		{
			white_string='whtList';
			white_string=white_string+i;		
			document.getElementById(white_string).disabled=false;
		}
		
	}
}

/**
 * @gsmNumber_blur  gsm失去焦点（判断连续、重复）
 * @author		wdz
 */
function gsmNumber_blur(num)
{
	var gsm_string='gsmComm';
	var i=0;
	var j=0;
	var next=0;
	gsm_string=gsm_string+num;
	document.getElementById(gsm_string).title="";
	document.getElementById("gsm_span").focus();
	var language_switch=$("#language_switch").val();
	if(1==document.getElementById("read_down").value)
	{
		document.getElementById("read_down").value=0;
		return ;
	}
	if(1==document.getElementById("import_down").value)
	{
		document.getElementById("import_down").value=0;
		return ;
	}
	
	for(i=1;i<11;i++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+i;
		if(validate_gsmComm_item(gsm_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+i;
		var base_value=document.getElementById(gsm_string).value;
		for(j=i+1;j<next;j++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+j;
			var current_value=document.getElementById(gsm_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	
	
	if(next !=11)  //存在无效、空  或者  重复项
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+next;
		var current_value=document.getElementById(gsm_string).value;
		var validate_result=validate_gsmComm_item(gsm_string);
		
		if("" == current_value)
		{
			if(next==10)
			{
				for(j=1;j<11;j++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+j;
					document.getElementById(gsm_string).readOnly=false;
				}
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+i;
					if(document.getElementById(gsm_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_gsmNumber').value=next;
					for(j=1;j<11;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						document.getElementById(gsm_string).readOnly=true;
					}
					if(0==language_switch)
					{
						gsm_string="联系人";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber(2,'确定','取消',gsm_string+'输入为空，为保证联系人列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						gsm_string="Contact ";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber(2,'OK','Cancel',"Deleting "+gsm_string+' deletes all followed contacts. Do you want to continue?','Prompt');
					}
					return ;
				}
			}
		}
		else if(0==validate_result)
		{
			if(next==10)
			{
				document.getElementById('invalidate_gsmNumber').value=next;
				for(j=1;j<11;j++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+j;
					document.getElementById(gsm_string).readOnly=true;
				}
				if(0==language_switch)
				{
					gsm_string="联系人";
					gsm_string=gsm_string+num;
					alarm_tip_gsmNumber_return(1,'确定','取消',gsm_string+"输入格式错误",'温馨提示');
				}
				else
				{
					gsm_string="Contact ";
					gsm_string=gsm_string+num;
					alarm_tip_gsmNumber_return(1,'OK','Cancel',"The format of "+gsm_string+" is wrong",'Prompt');
				}
				return ;
			}
			else
			{
				for(i=next+1;i<11;i++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+i;
					if(document.getElementById(gsm_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_gsmNumber').value=next;
					for(j=1;j<11;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						document.getElementById(gsm_string).readOnly=true;
					}
					if(0==language_switch)
					{
						gsm_string="联系人";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber(2,'确定','取消',gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表','温馨提示');
					}
					else
					{
						gsm_string="Contact ";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber_en(2,'OK','Cancel',"The format of "+gsm_string+" is wrong. If you continue, all followed contacts will be deleted. Do you want to continue?",'Prompt');
					}
					return ;
				}
				else
				{
					document.getElementById('invalidate_gsmNumber').value=next;
					for(j=1;j<11;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						document.getElementById(gsm_string).readOnly=true;
					}
					if(0==language_switch)
					{
						gsm_string="联系人";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber_return(1,'确定','取消',gsm_string+"输入格式错误",'温馨提示');
					}
					else
					{
						gsm_string="Contact ";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber_return(1,'OK','Cancel',"The format of "+gsm_string+" is wrong",'Prompt');
					}
					return ;
				}
			}
		}
		else
		{
			document.getElementById('invalidate_gsmNumber').value=next;
			for(j=1;j<11;j++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+j;
				document.getElementById(gsm_string).readOnly=true;
			}
			if(0==language_switch)
			{
				gsm_string="联系人";
				gsm_string=gsm_string+num;
				alarm_tip_gsmNumber_return(1,'确定','取消',gsm_string+"输入值已存在",'温馨提示');
			}
			else
			{
				gsm_string="Contact ";
				gsm_string=gsm_string+num;
				alarm_tip_gsmNumber_return(1,'OK','Cancel',"The "+gsm_string+" already exists",'Prompt');
			}
			return ;
		}
		for(i=1;i<next+1;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;		
			document.getElementById(gsm_string).disabled=false;			
		}
		for(i=next+1;i<11;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;		
			document.getElementById(gsm_string).disabled=true;
		}
	}
	else
	{
		for(i=1;i<next;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;		
			document.getElementById(gsm_string).disabled=false;			
		}
	}
}

/**
 * @validate_team  校验组呼列表
 * @author		wdz
 */
function validate_team()
{
	for(var i=1;i<11;i++)
    {
	    var team_string='team';
		team_string=team_string+i;
		
		if(validate_team_item(team_string)==0)
		{
			return i;
		}
	}
		
	return 0;
}


/**
 * @validate_freq_item  校验频率
 * @author		wdz
 */
///////验证频点组///////////
function validate_freq_item(team_num)
{
	var frePointNow = document.getElementById(team_num).value;
	var freq_channel = document.getElementById('freq_channel').value;
	var ret=/^([1-9][0-9]{1,2})([\.]([0-9]{1,6}))?$/;
	if(ret.test(frePointNow)== 0)
	{
		return 0;
	}
	else
	{
		if(freq_channel ==1)
		{
			if((frePointNow < 350)||(frePointNow> 400))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if(freq_channel ==2)
		{
			if((frePointNow < 410)||(frePointNow> 470))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
		
	}
}


/**
 * @validate_freq  校验频率
 * @author		wdz
 */
function validate_freq()
{
	
	for(var i=1;i<11;i++)
    {
	    var freq_string='frePoint';
		freq_string=freq_string+i;
		
		if(validate_freq_item(freq_string)==0)
		{
			
			return i;
		}
		
	}
	return 0;
}


/**
 * @validate_blklist_item  校验黑名单
 * @author		wdz
 */
///////验证黑名单///////////
function validate_blklist_item(team_num)
{
	var team1 = document.getElementById(team_num).value;
	var ret=/^\d{1,30}$/;
	if(ret.test(team1)== 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @validate_blklist  校验黑名单列表
 * @author		wdz
 */
function validate_blklist()
{
	
	for(var i=1;i<11;i++)
    {
	    var blklist_string='blkList';
		blklist_string=blklist_string+i;
		
		if(validate_blklist_item(blklist_string)==0)
		{
			return i;
		}
	}
	return 0;
}



/**
 * @validate_whtList_item  校验白名单
 * @author		wdz
 */
///////验证白名单///////////
function validate_whtList_item(team_num)
{
	var team1 = document.getElementById(team_num).value;
	var ret=/^\d{1,30}$/;
	if(ret.test(team1)== 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @validate_whtList  校验白名单列表
 * @author		wdz
 */
function validate_whtList()
{
	
	for(var i=1;i<11;i++)
    {
	    var whtList_string='whtList';
		whtList_string=whtList_string+i;
		
		if(validate_whtList_item(whtList_string)==0)
		{
			return i;
		}
	}
	return 0;
}


/**
 * @validate_gsmComm_item  校验gsm
 * @author		wdz
 */
///////验证GSM///////////
function validate_gsmComm_item(team_num)
{
	var team1 = document.getElementById(team_num).value;
	var ret1=/^\d{1,30}$/;
	var ret2=/^\+\d{1,29}$/;
	if((ret1.test(team1)== 0)&&(ret2.test(team1)== 0))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @validate_gsmComm  校验gsm列表
 * @author		wdz
 */
function validate_gsmComm()
{
	
	for(var i=1;i<11;i++)
    {
	    var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+i;
		
		if(validate_gsmComm_item(gsmComm_string)==0)
		{
			return i;
		}
	}
	return 0;
}


/**
 * @validate_radioID  校验中转台ID
 * @author		wdz
 */
///////验证信道机ID///////////
function validate_radioID()
{
	var radioID = document.getElementById('radioID').value;
	var ret=/^\d+$/;
	if(ret.test(radioID)== 0)
	{
		return 0;
	}
	else
	{
		if((radioID < 1)||(radioID> 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_interConctNum  校验中转台
 * @author		wdz
 */
///////验证互联组号///////////
function validate_interConctNum()
{
	var interConctNum = document.getElementById('interConctNum').value;
	var ret=/^\d+$/;
	if(ret.test(interConctNum)== 0)
	{
		return 0;
	}
	else
	{
		if((interConctNum < 1)||(interConctNum> 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_msgCtrlAdmID  校验短信控制管理员ID
 * @author		wdz
 */
///////验证短信控制管理员ID///////////
function validate_msgCtrlAdmID()
{
	var msgCtrlAdmID = document.getElementById('msgCtrlAdmID').value;
	var ret=/^\d+$/;
	if(ret.test(msgCtrlAdmID)== 0)
	{
		return 0;
	}
	else
	{
		if((msgCtrlAdmID < 1)||(msgCtrlAdmID> 16776415))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_gsm_call_interval  校验GSM主叫重呼间隔
 * @author		wdz
 */
///////验证GSM主叫重呼间隔///////////
function validate_gsm_call_interval()
{
	var gsm_call_interval = document.getElementById('gsm_call_interval').value;
	var ret=/^\d+$/;
	if(ret.test(gsm_call_interval)== 0)
	{
		return 0;
	}
	else
	{
		if((gsm_call_interval < 10)||(gsm_call_interval> 60))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/**
 * @validate_gsm_reboot_time  校验GSM模块异常重启时间
 * @author		wdz
 */
///////验证GSM模块异常重启时间///////////
function validate_gsm_reboot_time()
{
	var gsm_reboot_time = document.getElementById('gsm_reboot_time').value;
	var ret=/^\d+$/;
	if(ret.test(gsm_reboot_time)== 0)
	{
		return 0;
	}
	else
	{
		if((gsm_reboot_time < 60)||(gsm_reboot_time> 600))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_vol_delay  校验语音检测拖尾延时
 * @author		wdz
 */
///////验证语音检测拖尾延时///////////
function validate_vol_delay()
{
	var vol_delay = document.getElementById('vol_delay').value;
	var ret=/^\d+$/;
	if(ret.test(vol_delay)== 0)
	{
		return 0;
	}
	else
	{
		if((vol_delay < 1)||(vol_delay> 30))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_vol_energy_value  校验语音检测能量阈值
 * @author		wdz
 */
///////验证语音检测能量阈值///////////
function validate_vol_energy_value()
{
	var vol_energy_value = document.getElementById('vol_energy_value').value;
	var ret=/^\d+$/;
	if(ret.test(vol_energy_value)== 0)
	{
		return 0;
	}
	else
	{
		if((vol_energy_value < 1500)||(vol_energy_value> 4000))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


/**
 * @validate_power_value  校验发射功率
 * @author		wdz
 */
///////验证下拉列表///////////
function validate_power_value()
{
	var power_value = document.getElementById('power').value;
	var arr=["5","10","20"];
	if($.inArray(power_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_muteThreshold_value  校验静噪门限
 * @author		wdz
 */
function validate_muteThreshold_value()
{
	var muteThreshold_value = document.getElementById('muteThreshold').value;
	var arr=["700","1500","3000"];
	if($.inArray(muteThreshold_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_micVolume_value  校验MIC音量
 * @author		wdz
 */
function validate_micVolume_value()
{
	var micVolume_value = document.getElementById('micVolume').value;
	var arr=["1","2","3"];
	if($.inArray(micVolume_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_gsmVolume_value  校验GSM音量
 * @author		wdz
 */
function validate_gsmVolume_value()
{
	var gsmVolume_value = document.getElementById('gsmVolume').value;
	var arr=["1","2","3","4","5"];
	if($.inArray(gsmVolume_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_trumpetVolume_value  校验喇叭音量
 * @author		wdz
 */
function validate_trumpetVolume_value()
{
	var trumpetVolume_value = document.getElementById('trumpetVolume').value;
	var arr=["1","2","3","4","5"];
	if($.inArray(trumpetVolume_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_neighbor_switch_status_value  校验邻点查询使能
 * @author		wdz
 */
function validate_neighbor_switch_status_value()
{
	var neighbor_switch_status_value = document.getElementById('neighbor_switch_status').value;
	var arr=["0","1"];
	if($.inArray(neighbor_switch_status_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_neighbor_period_value  校验邻点周期
 * @author		wdz
 */
function validate_neighbor_period_value()
{
	var neighbor_period_value = document.getElementById('neighbor_period').value;
	var arr=["2","4","6","8","10","12","14","16","18","20"];
	if($.inArray(neighbor_period_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_neighbor_ai_switch_value  校验邻点上报使能
 * @author		wdz
 */
function validate_neighbor_ai_switch_value()
{
	var neighbor_ai_switch_value = document.getElementById('neighbor_ai_switch').value;
	var arr=["0","1"];
	if($.inArray(neighbor_ai_switch_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_stop_transmit  校验智能中转
 * @author		wdz
 */
function validate_stop_transmit()
{
	var stop_transmit = document.getElementById('stop_transmit_switch').value;
	var arr=["0","1"];
	if($.inArray(stop_transmit, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_dev_language_switch  校验语言开关
 * @author		wdz
 */
function validate_dev_language_switch()
{
	var dev_language_switch = document.getElementById('dev_language_switch').value;
	var arr=["0","1"];
	if($.inArray(dev_language_switch, arr)== -1)
	{
		return 0;
	}
	return 1;
}
/**
 * @validate_boot_mode  校验开机方式
 * @author		wdz
 */
function validate_boot_mode()
{
	var boot_mode_switch = document.getElementById('boot_mode_switch').value;
	var arr=["0","1"];
	if($.inArray(boot_mode_switch, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_reboot_strategy_switch  复位策略
 * @author		wdz
 */
function validate_reboot_strategy_switch()
{
	var reboot_strategy_switch = document.getElementById('reboot_strategy_switch').value;
	var arr=["0","1"];
	if($.inArray(reboot_strategy_switch, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_alarm_switch_status_value  校验告警上报使能
 * @author		wdz
 */
function validate_alarm_switch_status_value()
{
	var alarm_switch_status_value = document.getElementById('alarm_switch_status').value;
	var arr=["0","1"];
	if($.inArray(alarm_switch_status_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}
/**
 * @validate_alarm_show_switch_value  校验紧急警报显示使能
 * @author		wdz
 */
function validate_alarm_show_switch_value()
{
	var alarm_show_switch_value = document.getElementById('alarm_show_switch').value;
	var arr=["0","1"];
	if($.inArray(alarm_show_switch_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_gsm_alarm_show_switch_value  校验GSM告警使能
 * @author		wdz
 */
function validate_gsm_alarm_show_switch_value()
{
	var gsm_alarm_show_switch_value = document.getElementById('gsm_alarm_show_switch').value;
	var arr=["0","1"];
	if($.inArray(gsm_alarm_show_switch_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_radio_alarm_show_switch_value  校验中转台告警使能
 * @author		wdz
 */
function validate_radio_alarm_show_switch_value()
{
	var radio_alarm_show_switch_value = document.getElementById('radio_alarm_show_switch').value;
	var arr=["0","1"];
	if($.inArray(radio_alarm_show_switch_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_blkListConf_value  校验黑名单使能
 * @author		wdz
 */
function validate_blkListConf_value()
{
	var blkListConf_value = document.getElementById('blkListConf').value;
	var arr=["0","1"];
	if($.inArray(blkListConf_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_whtListConf_value  校验白名单使能
 * @author		wdz
 */
function validate_whtListConf_value()
{
	var whtListConf_value = document.getElementById('whtListConf').value;
	var arr=["0","1"];
	if($.inArray(whtListConf_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_msgCtrlFilter_value  校验短信控制过滤使能
 * @author		wdz
 */
function validate_msgCtrlFilter_value()
{
	var msgCtrlFilter_value = document.getElementById('msgCtrlFilter').value;
	var arr=["0","1"];
	if($.inArray(msgCtrlFilter_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_vol_start_threshold_value  校验语音检测启动门限
 * @author		wdz
 */
function validate_vol_start_threshold_value()
{
	var vol_start_threshold_value = document.getElementById('vol_start_threshold').value;
	var arr=["20","40","60","80","100","120","140","160","180","200"];
	if($.inArray(vol_start_threshold_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}

/**
 * @validate_vol_packet_time_value  校验语音包时长
 * @author		wdz
 */
function validate_vol_packet_time_value()
{
	var vol_packet_time_value = document.getElementById('vol_packet_time').value;
	var arr=["20","60"];
	if($.inArray(vol_packet_time_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_terminal_compatible_value  校验终端兼容使能
 * @author		wdz
 */
function validate_terminal_compatible_value()
{
	var terminal_compatible_value = document.getElementById('terminal_compatible').value;
	var arr=["0","1"];
	if($.inArray(terminal_compatible_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_work_mode_value  校验工作模式
 * @author		wdz
 */
function validate_work_mode_value()
{
	var work_mode_value = document.getElementById('work_mode').value;
	var arr=["0","1"];
	if($.inArray(work_mode_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_voice_mode_value  校验语音编码格式
 * @author		wdz
 */
function validate_voice_mode_value()
{
	var voice_mode_value = document.getElementById('voice_mode').value;
	var arr=["206","207"];
	if($.inArray(voice_mode_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @validate_linkTime_value  校验链路保持时间
 * @author		wdz
 */
function validate_linkTime_value()
{
	var linkTime_value = document.getElementById('linkTime').value;
	var ret=/^[500,1000,1500,2000,2500,3000]$/;
	var arr=["500","1000","1500","2000","2500","3000"];
	if($.inArray(linkTime_value, arr)== -1)
	{
		return 0;
	}
	return 1;
}


/**
 * @convert_data_to_def  除写入操作外，其它提交表单操作检测不合法，置默认值
 * @author		wdz
 */
function convert_data_to_def()
{
	var mode=document.getElementById("current_mode").value; 
	
	if(validate_linkIp()==0)
	{
		document.getElementById('linkIP').value ="";
    }
	if(validate_mask()==0)
	{
		document.getElementById('mask').value = "";
    }
	if(validate_gateWay()==0)
	{
		document.getElementById('gateWay').value="";
    }
	if(validate_centerIP()==0)
	{
		document.getElementById('centerIP').value ="";
    }
	if(validate_centerID()==0)
	{
		document.getElementById('centerID').value =4051;
    }
	if(validate_epkID()==0)
	{
		document.getElementById('epkID').value = 0;
    }
	if(validate_intercomID()==0)
	{
		document.getElementById('intercomID').value =1;
    }
	if(validate_power_value()==0)
	{
		document.getElementById('power').value =5;
    }
	if(validate_epkCode()==0)
	{
		document.getElementById('epkCode').value=1;
    }
	if(validate_termCode()==0)
	{
		document.getElementById('termCode').value=5;
    }
	if(validate_muteThreshold_value()==0)
	{
		document.getElementById('muteThreshold').value=3000;
    }
	if(validate_micVolume_value()==0)
	{
		document.getElementById('micVolume').value=2;
    }
	if(validate_gsmVolume_value()==0)
	{
		document.getElementById('gsmVolume').value=3;
    }
	if(validate_trumpetVolume_value()==0)
	{
		document.getElementById('trumpetVolume').value=3;
    }
	/*
	if(validate_termDizzyOverTime()==0)
	{
		document.getElementById('termDizzyOverTime').value =2000;
    }
	*/
	if(validate_devCallTimeout()==0)
	{
		document.getElementById('devCallTimeout').value =60;
    }
	/*
	if(validate_neighbor_period()==0)
	{
		document.getElementById('neighbor_period').value =2;
    }
	*/
	/*
	if(validate_close_transmit_threshold()==0)
	{
		document.getElementById('close_transmit_threshold').value =0;
    }
	if(validate_resume_transmit_threshold()==0)
	{
		document.getElementById('resume_transmit_threshold').value =0;
    }
	*/
	if(validate_neighbor_switch_status_value()==0)
	{
		document.getElementById('neighbor_switch_status').value =1;
    }
	if(validate_neighbor_period_value()==0)
	{
		document.getElementById('neighbor_period').value =2;
    }
	if(validate_neighbor_ai_switch_value()==0)
	{
		document.getElementById('neighbor_ai_switch').value =0;
    }
	if(validate_stop_transmit()==0)
	{
		document.getElementById('stop_transmit_switch').value =0;
    }
	if(validate_dev_language_switch()==0)
	{
		document.getElementById('dev_language_switch').value =0;
    }
	if(validate_boot_mode()==0)
	{
		document.getElementById('boot_mode_switch').value =0;
    }
	if(validate_reboot_strategy_switch()==0)
	{
		document.getElementById('reboot_strategy_switch').value =0;
    }
	if(validate_alarm_switch_status_value()==0)
	{
		document.getElementById('alarm_switch_status').value =0;
    }
	if(validate_alarm_count()==0)
	{
		document.getElementById('alarm_count').value =0;
    }
	if(validate_alarm_show_switch_value()==0)
	{
		document.getElementById('alarm_show_switch').value =1;
    }
	if(validate_gsm_alarm_show_switch_value()==0)
	{
		document.getElementById('gsm_alarm_show_switch').value =1;
    }
	if(validate_radio_alarm_show_switch_value()==0)
	{
		document.getElementById('radio_alarm_show_switch').value =1;
    }
	/*
	if(validate_teamIdNow()==0)
	{
		document.getElementById('teamIdNow').value ="";
    }*/
	var team_return=validate_team();
	if(team_return ==0)
	{
		team_return=11;
	}
		
	for(var i=1;i<team_return;i++)
	{
		var team_num='team';
		team_num=team_num+i;
		var base_value=document.getElementById(team_num).value;
		
		for(var j=i+1;j<team_return;j++)
		{
			team_num='team';
			team_num=team_num+j;
			var current_value=document.getElementById(team_num).value;
			if(base_value==current_value)
			{
				team_return=j;
				break;
			}
		}
	}
	for(;team_return<11;team_return++)
	{
		var team_num='team';
		team_num=team_num+team_return;
		document.getElementById(team_num).value ="";
	}
		
	var freq_return=validate_freq();
	if(freq_return==0)
	{
		freq_return=11;
	}
	for(var i=1;i<freq_return;i++)
	{
		var freq_string='frePoint';
		freq_string=freq_string+i;
		var base_value=document.getElementById(freq_string).value;
		
		for(var j=i+1;j<freq_return;j++)
		{
			freq_string='frePoint';
			freq_string=freq_string+j;
			var current_value=document.getElementById(freq_string).value;
			
			if(parseFloat(base_value)== parseFloat(current_value))
			{
				freq_return=j;
				break;
			}
		}
	}
	for(;freq_return<11;freq_return++)
	{
		var freq_num='frePoint';
		freq_num=freq_num+freq_return;
		document.getElementById(freq_num).value ="";
	}
	
	var gsmComm_return=validate_gsmComm();
	if(gsmComm_return==0)
	{
		gsmComm_return=11;
	}
	for(var i=1;i<gsmComm_return;i++)
	{
		var gsmNumber_string='gsmComm';
		gsmNumber_string=gsmNumber_string+i;
		var base_value=document.getElementById(gsmNumber_string).value;
		
		for(var j=i+1;j<gsmComm_return;j++)
		{
			gsmNumber_string='gsmComm';
			gsmNumber_string=gsmNumber_string+j;
			var current_value=document.getElementById(gsmNumber_string).value;
			
			if(base_value==current_value)
			{
				gsmComm_return=j;
				break;
			}
		}
	}
	for(;gsmComm_return<11;gsmComm_return++)
	{
		var gsmComm_num='gsmComm';
		gsmComm_num=gsmComm_num+gsmComm_return;
		document.getElementById(gsmComm_num).value ="";
	} 
	
	if(mode==1)
	{
		if(validate_blkListConf_value()==0)
		{
			document.getElementById('blkListConf').value =0;
		}
		if(validate_whtListConf_value()==0)
		{
			document.getElementById('whtListConf').value =0;
		}
		
		var blklist_return=validate_blklist();
		if(blklist_return==0)
		{
			blklist_return=11;
		}
		for(var i=1;i<blklist_return;i++)
		{
			var blackNumber_string='blkList';
			blackNumber_string=blackNumber_string+i;
			var base_value=document.getElementById(blackNumber_string).value;
		
			for(var j=i+1;j<blklist_return;j++)
			{
				blackNumber_string='blkList';
				blackNumber_string=blackNumber_string+j;
				var current_value=document.getElementById(blackNumber_string).value;
			
				if(base_value==current_value)
				{
					blklist_return=j;
					break;
				}
			}
		}
		for(;blklist_return<11;blklist_return++)
		{
			var blklist_num='blkList';
			blklist_num=blklist_num+blklist_return;
			document.getElementById(blklist_num).value ="";
		} 
		var whtList_return=validate_whtList();
		if(whtList_return ==0)
		{
			whtList_return=11;
		}
		for(var i=1;i<whtList_return;i++)
		{
			var whiteNumber_string='whtList';
			whiteNumber_string=whiteNumber_string+i;
			var base_value=document.getElementById(whiteNumber_string).value;
		
			for(var j=i+1;j<whtList_return;j++)
			{
				whiteNumber_string='whtList';
				whiteNumber_string=whiteNumber_string+j;
				var current_value=document.getElementById(whiteNumber_string).value;
			
				if(base_value==current_value)
				{
					whtList_return=j;
					break;
				}
			}
		}
		for(;whtList_return<11;whtList_return++)
		{
			var whtList_num='whtList';
			whtList_num=whtList_num+whtList_return;
			document.getElementById(whtList_num).value ="";
		}	
		if(validate_radioIP()==0)
		{
			document.getElementById('radioIP').value ="";
		}
		if(validate_radioID()==0)
		{
			document.getElementById('radioID').value =100;
		}
		if(validate_interConctNum()==0)
		{
			document.getElementById('interConctNum').value=1000;
		}
		if(validate_msgCtrlAdmID()==0)
		{
			document.getElementById('msgCtrlAdmID').value =101;
		}
		if(validate_msgCtrlFilter_value()==0)
		{
			document.getElementById('msgCtrlFilter').value =0;
		}
		if(validate_vol_start_threshold_value()==0)
		{
			document.getElementById('vol_start_threshold').value =80;
		}
		if(validate_vol_packet_time_value()==0)
		{
			document.getElementById('vol_packet_time').value =60;
		}
		if(validate_terminal_compatible_value()==0)
		{
			document.getElementById('terminal_compatible').value =1;
		}
		if(validate_work_mode_value()==0)
		{
			document.getElementById('work_mode').value =0;
		}
		if(validate_voice_mode_value()==0)
		{
			document.getElementById('voice_mode').value =207;
		}
		if(validate_linkTime_value()==0)
		{
			document.getElementById('linkTime').value =500;
		}
		if(validate_devOverTime()==0)
		{
			document.getElementById('devOverTime').value =5;
		}
		if(validate_termGpsOverTime()==0)
		{
			document.getElementById('termGpsOverTime').value =10;
		}
		if(validate_close_transmit_rssi_threshold()==0)
		{
			document.getElementById('close_transmit_rssi_threshold').value =-60;
		}
		if(validate_resume_transmit_rssi_threshold()==0)
		{
			document.getElementById('resume_transmit_rssi_threshold').value =-70;
		}
		if(validate_tempratue_alarm_start_threshold()==0)
		{
			document.getElementById('tempratue_alarm_start_threshold').value =85;
		}
		if(validate_tempratue_alarm_close_threshold()==0)
		{
			document.getElementById('tempratue_alarm_close_threshold').value =80;
		}
		if(validate_gsm_call_interval()==0)
		{
			document.getElementById('gsm_call_interval').value =30;
		}
		if(validate_gsm_reboot_time()==0)
		{
			document.getElementById('gsm_reboot_time').value =120;
		}
		if(validate_vol_delay()==0)
		{
			document.getElementById('vol_delay').value =3;
		}
		if(validate_vol_energy_value()==0)
		{
			document.getElementById('vol_energy_value').value =2000;
		}
	}
	else if(mode == 0)
	{
		var ret=/^[0-9a-zA-Z]{5,20}$/;
		var user=document.getElementById("old_user").value;
		var old_code=document.getElementById("old_code").value;
		var new_user=document.getElementById("new_user").value;
		var new_code=document.getElementById("new_code").value;
		var confirm_code=document.getElementById("confirm_code").value;
		
		if((user!="")&&(ret.test(user)== 0))
		{
			document.getElementById("old_user").value="";
		}
		if((new_user!="")&&(ret.test(new_user)== 0))
		{
			document.getElementById("new_user").value="";
		}
		if((old_code!="")&&(ret.test(old_code)== 0))
		{
			document.getElementById("old_code").value="";
		}
		if((new_code!="")&&(ret.test(new_code)== 0))
		{
			document.getElementById("new_code").value="";
		}
		if((confirm_code!="")&&(ret.test(confirm_code)== 0))
		{
			document.getElementById("confirm_code").value="";
		}
	}
	
	
}


/**
 * @convert_data_to_def_import  导入操作检测不合法，置默认值
 * @author		wdz
 */
function convert_data_to_def_import()
{
	var mode=document.getElementById("current_mode").value; 
	
	if(validate_linkIp()==0)
	{
		document.getElementById('linkIP').value ="";
		document.getElementById("linkIP").style.borderColor="#aaa";
    }
	if(validate_mask()==0)
	{
		document.getElementById('mask').value = "";
		document.getElementById("mask").style.borderColor="#aaa";
    }
	if(validate_gateWay()==0)
	{
		document.getElementById('gateWay').value="";
		document.getElementById("gateWay").style.borderColor="#aaa";
    }
	if(validate_centerIP()==0)
	{
		document.getElementById('centerIP').value ="";
		document.getElementById("centerIP").style.borderColor="#aaa";
    }
	if(validate_centerID()==0)
	{
		document.getElementById('centerID').value =4051;
		document.getElementById("centerID").style.borderColor="#aaa";
    }
	if(validate_epkID()==0)
	{
		document.getElementById('epkID').value = 0;
		document.getElementById("epkID").style.borderColor="#aaa";
    }
	if(validate_intercomID()==0)
	{
		document.getElementById('intercomID').value =1;
		document.getElementById("intercomID").style.borderColor="#aaa";
    }
	if(validate_power_value()==0)
	{
		document.getElementById('power').value =5;
    }
	if(validate_epkCode()==0)
	{
		document.getElementById('epkCode').value=1;
		document.getElementById("epkCode").style.borderColor="#aaa";
    }
	if(validate_termCode()==0)
	{
		document.getElementById('termCode').value=5;
		document.getElementById("termCode").style.borderColor="#aaa";
    }
	if(validate_muteThreshold_value()==0)
	{
		document.getElementById('muteThreshold').value=3000;
    }
	if(validate_micVolume_value()==0)
	{
		document.getElementById('micVolume').value=2;
    }
	if(validate_gsmVolume_value()==0)
	{
		document.getElementById('gsmVolume').value=3;
    }
	if(validate_trumpetVolume_value()==0)
	{
		document.getElementById('trumpetVolume').value=3;
    }
	if(validate_devCallTimeout()==0)
	{
		document.getElementById('devCallTimeout').value =60;
		document.getElementById("devCallTimeout").style.borderColor="#aaa";
    }
	if(validate_neighbor_switch_status_value()==0)
	{
		document.getElementById('neighbor_switch_status').value =1;
    }
	if(validate_neighbor_period_value()==0)
	{
		document.getElementById('neighbor_period').value =2;
		document.getElementById("neighbor_period").style.borderColor="#aaa";
    }
	if(validate_neighbor_ai_switch_value()==0)
	{
		document.getElementById('neighbor_ai_switch').value =0;
    }
	if(validate_stop_transmit()==0)
	{
		document.getElementById('stop_transmit_switch').value =0;
    }
	if(validate_dev_language_switch()==0)
	{
		document.getElementById('dev_language_switch').value =0;
    }
	if(validate_boot_mode()==0)
	{
		document.getElementById('boot_mode_switch').value =0;
    }
	if(validate_reboot_strategy_switch()==0)
	{
		document.getElementById('reboot_strategy_switch').value =0;
    }
	if(validate_alarm_switch_status_value()==0)
	{
		document.getElementById('alarm_switch_status').value =0;
    }
	if(validate_alarm_count()==0)
	{
		document.getElementById('alarm_count').value =0;
		document.getElementById("alarm_count").style.borderColor="#aaa";
    }
	if(validate_alarm_show_switch_value()==0)
	{
		document.getElementById('alarm_show_switch').value =1;
    }
	if(validate_gsm_alarm_show_switch_value()==0)
	{
		document.getElementById('gsm_alarm_show_switch').value =1;
    }
	if(validate_radio_alarm_show_switch_value()==0)
	{
		document.getElementById('radio_alarm_show_switch').value =1;
    }
	
	if(mode==1)
	{
		if(validate_blkListConf_value()==0)
		{
			document.getElementById('blkListConf').value =0;
		}
		if(validate_whtListConf_value()==0)
		{
			document.getElementById('whtListConf').value =0;
		}
		if(validate_radioIP()==0)
		{
			document.getElementById('radioIP').value ="";
			document.getElementById("radioIP").style.borderColor="#aaa";
		}
		if(validate_radioID()==0)
		{
			document.getElementById('radioID').value =100;
			document.getElementById("radioID").style.borderColor="#aaa";
		}
		if(validate_interConctNum()==0)
		{
			document.getElementById('interConctNum').value=1000;
			document.getElementById("interConctNum").style.borderColor="#aaa";
		}
		if(validate_msgCtrlAdmID()==0)
		{
			document.getElementById('msgCtrlAdmID').value =101;
			document.getElementById("msgCtrlAdmID").style.borderColor="#aaa";
		}
		if(validate_msgCtrlFilter_value()==0)
		{
			document.getElementById('msgCtrlFilter').value =0;
		}
		
		if(validate_vol_start_threshold_value()==0)
		{
			document.getElementById('vol_start_threshold').value =80;
		}
		if(validate_vol_packet_time_value()==0)
		{
			document.getElementById('vol_packet_time').value =60;
		}
		if(validate_terminal_compatible_value()==0)
		{
			document.getElementById('terminal_compatible').value =1;
		}
		if(validate_work_mode_value()==0)
		{
			document.getElementById('work_mode').value =0;
		}
		if(validate_voice_mode_value()==0)
		{
			document.getElementById('voice_mode').value =207;
		}
		if(validate_linkTime_value()==0)
		{
			document.getElementById('linkTime').value =500;
		}
		if(validate_devOverTime()==0)
		{
			document.getElementById('devOverTime').value =5;
			document.getElementById("devOverTime").style.borderColor="#aaa";
		}
		if(validate_termGpsOverTime()==0)
		{
			document.getElementById('termGpsOverTime').value =10;
			document.getElementById("termGpsOverTime").style.borderColor="#aaa";
		}
		if(validate_close_transmit_rssi_threshold()==0)
		{
			document.getElementById('close_transmit_rssi_threshold').value =-60;
			document.getElementById("close_transmit_rssi_threshold").style.borderColor="#aaa";
		}
		if(validate_resume_transmit_rssi_threshold()==0)
		{
			document.getElementById('resume_transmit_rssi_threshold').value =-70;
			document.getElementById("resume_transmit_rssi_threshold").style.borderColor="#aaa";
		}
		if(validate_tempratue_alarm_start_threshold()==0)
		{
			document.getElementById('tempratue_alarm_start_threshold').value =85;
			document.getElementById("tempratue_alarm_start_threshold").style.borderColor="#aaa";
		}
		if(validate_tempratue_alarm_close_threshold()==0)
		{
			document.getElementById('tempratue_alarm_close_threshold').value =80;
			document.getElementById("tempratue_alarm_close_threshold").style.borderColor="#aaa";
		}
		if(validate_gsm_call_interval()==0)
		{
			document.getElementById('gsm_call_interval').value =30;
			document.getElementById("gsm_call_interval").style.borderColor="#aaa";
		}
		if(validate_gsm_reboot_time()==0)
		{
			document.getElementById('gsm_reboot_time').value =120;
			document.getElementById("gsm_reboot_time").style.borderColor="#aaa";
		}
		if(validate_vol_delay()==0)
		{
			document.getElementById('vol_delay').value =3;
			document.getElementById("vol_delay").style.borderColor="#aaa";
		}
		if(validate_vol_energy_value()==0)
		{
			document.getElementById('vol_energy_value').value =2000;
			document.getElementById("vol_energy_value").style.borderColor="#aaa";
		}
	}
	else if(mode == 0)
	{
		var ret=/^[0-9a-zA-Z]{5,20}$/;
		var user=document.getElementById("old_user").value;
		var old_code=document.getElementById("old_code").value;
		var new_user=document.getElementById("new_user").value;
		var new_code=document.getElementById("new_code").value;
		var confirm_code=document.getElementById("confirm_code").value;
		
		if((user!="")&&(ret.test(user)== 0))
		{
			document.getElementById("old_user").value="";
		}
		if((new_user!="")&&(ret.test(new_user)== 0))
		{
			document.getElementById("new_user").value="";
		}
		if((old_code!="")&&(ret.test(old_code)== 0))
		{
			document.getElementById("old_code").value="";
		}
		if((new_code!="")&&(ret.test(new_code)== 0))
		{
			document.getElementById("new_code").value="";
		}
		if((confirm_code!="")&&(ret.test(confirm_code)== 0))
		{
			document.getElementById("confirm_code").value="";
		}
	}
	
	
}

/**
 * @clear_invalidate_list_data  导入操作检测组呼、频率列表等不合法，清空
 * @author		wdz
 */
function clear_invalidate_list_data()
{
	var mode=document.getElementById("current_mode").value; 
	
	//清除无效组呼列表，更新当前组
	showTeam();
	var team_current_value=document.getElementById("teamIdNow").value;
	add_id_option();  //更新当前组列表
	if(team_current_value !="")
	{
		$("#teamIdNow option").each(function()
		{
			var tmp_value = $(this).val();
			if(parseInt(tmp_value) == parseInt(team_current_value))
			{
				document.getElementById("teamIdNow").value=team_current_value;
			}
		})
	}
	
	//清除无效频率列表，更新当前频率
	showFreq();
	var freq_current_value=document.getElementById("frePointNow").value;
	add_freq_option();	
	if(freq_current_value!="")
	{
		$("#frePointNow option").each(function() {
		var tmp_value = $(this).val();
		if(parseFloat(tmp_value) == parseFloat(freq_current_value))
		{
			document.getElementById("frePointNow").value=parseFloat(freq_current_value);
		}
		});
	}
	//清除无效gsm列表
	showgsmComm();
	if(mode==1)
	{
		showBlklist();
		showWhtList();
	}
}
/**
 * @validate_IdNow_in_team  检验当前组在不在组列表中
 * @author		wdz
 */
function validate_IdNow_in_team()
{
	var teamIdNow = document.getElementById('teamIdNow').value;
	var result=false;
	var i=0;

	for(i=1;i<11;i++)
	{
		var team="team"+i;
		var team_string=document.getElementById(team).value;
		if(team_string=="")
		{
			break;
		}
		else if(teamIdNow == team_string)
		{
			result=true;
			break;
		}
	}
	return result;
}

/**
 * @validate_freqNow_in_team  检验当前频率在不在频率列表中
 * @author		wdz
 */
function validate_freqNow_in_team()
{
	var freqNow = document.getElementById('frePointNow').value;
	var result=false;
	var i=0;
	console.log("aa"+freqNow);
	var ret=/^\d+$/;
	
	if(ret.test(freqNow)== 0)
	{
		return result;
	}
	
	for(i=1;i<11;i++)
	{
		var team="frePoint"+i;
		var team_string=document.getElementById(team).value;
		if(team_string=="")
		{
			break;
		}
		else if(freqNow == team_string*1000000)
		{
			result=true;
			break;
		}
	}
	return result;
}

/**
 * @validate_code  检验链路色码与终端色码是否一致
 * @author		wdz
 */
function validate_code()
{
	var epkCode = document.getElementById('epkCode').value;
	var termCode = document.getElementById('termCode').value;
	if(epkCode == termCode)
	{
		return false;
	}
	return true;
}


/**
 * @add_id_option  将有效组列表动态添加到当前组下拉列表中
 * @author		wdz
 */
function add_id_option()
{
	var index=11;
	var language_switch=$("#language_switch").val();
	for(var i=1;i<11;i++)
	{
		var team_string='team';
		team_string=team_string+i;
		
		if(validate_team_item(team_string)==0)
		{
			index=i;
			break;
		}
	}
	
	
	if(index ==1)
	{
		$("#teamIdNow").empty();
	}
	else 
	{
		$("#teamIdNow").empty();
		for(var i=1;i<index;i++)
		{
			var exist=false;
			var team_string='team';
		    team_string=team_string+i;
			var team_value=document.getElementById(team_string).value;
			
			$("#teamIdNow option").each(function() {
				tmp_value = $(this).val();
				if(parseInt(tmp_value) == parseInt(team_value))
				{
					exist=true;
				}
			});
			if(exist == false)
			{
				if(language_switch==0)
				{
					$("#teamIdNow").append( "<option value="+team_value+">"+"组"+i+"--"+team_value+"</option>" );
				}
				else
				{
					$("#teamIdNow").append( "<option value="+team_value+">"+"Group"+i+"--"+team_value+"</option>" );
				}
			}
			
		}
	}
}


/**
 * @add_freq_option  将有效频率列表动态添加到当前频率下拉列表中
 * @author		wdz
 */
function add_freq_option()
{
	var index=11;
	var language_switch=$("#language_switch").val();
	for(var i=1;i<11;i++)
	{
		var team_string='frePoint';
		team_string=team_string+i;
		
		if(validate_freq_item(team_string)==0)
		{
		//		document.getElementById(team_string).value="";
			index=i;
				/*
				i++;
				for(;i<11;i++)
				{   
					team_string='frePoint';
					team_string=team_string+i;
					document.getElementById(team_string).value="";
					document.getElementById(team_string).disabled=true;
				}
				*/
			break ;
		}
	}
	if(index ==1)
	{
		$("#frePointNow").empty();
	//	$("#teamIdNow").append( "<option value=\"0\">"+" "+"</option>" );
	}
	else 
	{
		$("#frePointNow").empty();
		for(var i=1;i<index;i++)
		{
			var exist=false;
			var team_string='frePoint';
		    team_string=team_string+i;
			var team_value=document.getElementById(team_string).value;
			
			$("#frePointNow option").each(function() {
				tmp_value = $(this).val();
				if(parseFloat(tmp_value) == parseFloat(team_value)*1000000)
				{
					exist=true;
				}
			});
			if(exist == false)
			{
				var tmp=parseFloat(team_value)*1000000;
				if(language_switch==0)
				{
					$("#frePointNow").append( "<option value="+tmp+">"+"频率"+i+"--"+team_value+"</option>" );	
				}
				else
				{
					$("#frePointNow").append( "<option value="+tmp+">"+"Frequency"+i+"--"+team_value+"</option>" );	
				}	
			}
			
		}
	}
}

/**
 * @alarm_tip_user  原用户名输入无效提示框
 * @author		wdz
 */
function alarm_tip_user(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("old_user").style.borderColor="red";
																			document.getElementById("old_user").focus();
																			
																			}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}


/**
 * @alarm_tip_code  原密码输入无效提示框
 * @author		wdz
 */
function alarm_tip_code(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("old_code").style.borderColor="red";
																			document.getElementById("old_code").focus();
																			
																			}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}


/**
 * @alarm_tip_confirm_code  密码输入不一致提示框
 * @author		wdz
 */
function alarm_tip_confirm_code(btns_num,btn1,btn2,option_info,option_title){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,btn : [btn1,btn2],type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("confirm_code").style.borderColor="red";
																			document.getElementById("confirm_code").focus();
																			
																			}},
				title : [option_title,true],
				offset: ['300px', '50%']		
				});
}


/**
 * @modify_code  修改用户信息，之前校验函数
 * @author		wdz
 */
function modify_code()
{
	var ret=/^[0-9a-zA-Z]{5,20}$/;
	var user=document.getElementById("old_user").value;
	var old_code=document.getElementById("old_code").value;
	var new_user=document.getElementById("new_user").value;
	var new_code=document.getElementById("new_code").value;
	var confirm_code=document.getElementById("confirm_code").value;
	var web_user=document.getElementById("web_user").value;
	var web_code=document.getElementById("web_code").value;
	var language_switch=$("#language_switch").val();   //0：中文；1：英文
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
    if(freq_channel==0)
    {
	   if(language_switch==0)
	   {
		   alarm_tip(1,'确定','取消','设备支持频段与应用程序不匹配，请更新程序','温馨提示');
		   return;
	   }
	   else
	   {
		   alarm_tip_add(1,'OK','Cancel','The frequency band supported by the system does not match the application. Please update the application.','Prompt');
		   return;
	   }
	   
    }
	///验证原用户名/////
	if(user != web_user)
	{
		if(0==language_switch)
		{
			alarm_tip_user(1,'确定','取消','该用户名不存在','温馨提示');
		}
		else
		{
			alarm_tip_user(1,'OK','Cancel','The user name does not exist','Prompt');
		}
		return;
	}
	else
	{
		document.getElementById("old_user").style.borderColor="#aaa";
	}
	
	if(ret.test(new_user)== 0)
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','新用户名输入无效','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The new user name is invalid','Prompt');
		}
		
		return;	
	}
	if(old_code != web_code)
	{
		if(0==language_switch)
		{
			alarm_tip_code(1,'确定','取消','密码错误','温馨提示');
		}
		else
		{
			alarm_tip_code(1,'OK','Cancel','Incorrect password','Prompt');
		}
		return;
	}
	else
	{
		document.getElementById("old_code").style.borderColor="#aaa";
	}
	if(ret.test(new_code)== 0)
	{
		if(0==language_switch)
		{
			alarm_tip(1,'确定','取消','新密码输入无效','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The new password is invalid','Prompt');
		}
		
		return;	
	}
	if(new_code != confirm_code)
	{
		if(0==language_switch)
		{
			alarm_tip_confirm_code(1,'确定','取消','两次密码输入不一致','温馨提示');
		}
		else
		{
			alarm_tip_confirm_code(1,'OK','Cancel','The passwords do not match','Prompt');
		}
		return;
	}
	else
	{
		document.getElementById("confirm_code").style.borderColor="#aaa";
	}
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	if(document.getElementById("current_mode").value==1)
	{
		document.getElementById('set_code').click();
		if(0==language_switch)
		{
			layer.load('保存用户信息中，请等待。。。。');
		}
		else
		{
			layer.load('Saving user information, please wait......');
		}
		
	}
	else
	{
		document.getElementById('set_code_validate').click();
	}
	
	//layer.load('保存用户信息中，请等待。。。。');
}

function  set_code(){
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById('set_code').click();
	var language_switch=$("#language_switch").val();
	if(0==language_switch)
	{
		layer.load('保存用户信息中，请等待。。。。');
	}
	else
	{
		layer.load('Saving user information, please wait......');
	}
}


/**
 * @brief    登录判断（输入合不合法）
 * @author		wdz
 * @bug
 */

 /**
 * @brief    登录判断（输入合不合法）
 * @author		wdz
 * @bug
 */

 function  log(){

	$("#log").click(function(){
		var user=$("#用户名").val();
		var code=$("#密码").val();
		var language_switch=$("#language_switch").val();
		var ret=/^[0-9a-zA-Z]{5,20}$/;
		
		if((ret.test(user)== 0)||(ret.test(code)== 0))
		{
			if(0==language_switch)
			{
				alarm_tip(1,"确定",'取消','用户名与密码不匹配','温馨提示');
			}
			else
			{
				alarm_tip(1,'OK','Cancel','The user name and password do not match','Prompt');
			}
			
		}
		else 
		{
			$("#submit").click();
		}
	})

}


/**
 * @chang_text  语言切换文字显示
 * @author		wdz
 */
function chang_text()
{
	var language_switch=$("#language_switch").val();
	if(0==language_switch)
	{
		$("#h1_text").text("E-pack100 单机网络管理系统");
		$("#log").text("登录");
		$("#p_text").text("版权所有 © 1993-2017 海能达通信股份有限公司");
	}
	else
	{
		$("#h1_text").text("E-pack100 Standalone Network Management System");
		$("#log").text("Login");
		$("#p_text").text("Copyright Reserved © 1993-2017 Hytera Communications Corporation Limited");
	}
}



/**
 * @log_type  登录输入格式合法，登陆验证
 * @author		wdz
 */
function  log_type(flag,user,code,language_switch){
	
	document.getElementById("用户名").value=user;
	document.getElementById("密码").value=code;

	if(0==language_switch)
	{
		$("#h1_text").text("E-pack100 单机网络管理系统");
		$("#log").text("登录");
		$("#p_text").text("版权所有 © 1993-2017 海能达通信股份有限公司");
	}
	else
	{
		$("#h1_text").text("E-pack100 Standalone Network Management System");
		$("#log").text("Login");
		$("#p_text").text("Copyright Reserved © 1993-2017 Hytera Communications Corporation Limited");
	}
	document.getElementById("language_switch").value=language_switch;
	
	if(flag ==1)
	{
		//  常规模式
		$("#normal_mode").click();
	}
	else if(flag ==2 )
	{
		//  工程模式
		$("#engineer_pattern").click();
	}
	else if(flag ==3)
	{
		
		if(0==language_switch)
		{
			alarm_tip(1,"确定",'取消','用户名与密码不匹配','温馨提示');
		}
		else
		{
			alarm_tip(1,'OK','Cancel','The user name and password do not match','Prompt');
		}
	}
}


/**
 * @brief    屏蔽回车键
 * @author		wdz
 * @bug
 */
function Enter(event){   
   
    event=window.event||event|| event.which;;
	if(event.keyCode == 13)        
	{    
		event.returnValue=false;        
	} 
	
	 
	/*
	if ( (event.altKey==1)&&   
    (  (event.keyCode==37)|| //屏蔽 Alt+ 方向键 ←   
       (event.keyCode==39) )  ){ //屏蔽 Alt+ 方向键 →    
        event.returnValue=false; 
    } 

   if ( (event.keyCode==116)|| //屏蔽 F5 刷新键   
   　　(event.keyCode==112)|| (event.keyCode==113)||(event.keyCode==114)||(event.keyCode==115)||//屏蔽 F1 刷新键   
    　(event.keyCode==117)||(event.keyCode==118)||(event.keyCode==119)||(event.keyCode==120)||((event.ctrlKey)&& event.keyCode==82))
	{ //Ctrl + R    
    　	event.keyCode = 0;
		event.returnValue=false; 
    }  
   if ((event.ctrlKey )&&(event.keyCode==78)) //屏蔽 Ctrl+n   
   {    
    　	event.returnValue=false; 
   }  
   if ((event.shiftKey)&&(event.keyCode==121)) //屏蔽 shift+F10   
   {     
    　	event.returnValue=false;
   }
   if (event.srcElement.tagName == "A" && (event.shiftKey ==1))   
　 {     
    　	event.returnValue=false;  //屏蔽 shift 加鼠标左键新开一网页   
　 }  
       
    if ((event.altKey)&&(event.keyCode==115))
	{ //屏蔽Alt+F4         
    　	event.returnValue=false;
	}   
	*/
}

/**
 * @show_title    配置项增加title
 * @author		wdz
 * @bug
 */
/**
 * @show_title    配置项增加title
 * @author		wdz
 * @bug
 */
function  show_title(){
	var mode=document.getElementById("current_mode").value; 
	var language_switch=$("#language_switch").val();
	var freq_channel=document.getElementById('freq_channel').value;     //频段标志  1：350-400   2：410-470
	if(language_switch ==0)
	{
		///////////////网络参数////////////////
		document.getElementById('linkIP').title="本机IP地址";
		document.getElementById('mask').title="本机子网掩码";
		document.getElementById('gateWay').title="本机网关";
		document.getElementById('centerIP').title="控制中心IP地址";
		document.getElementById('centerID').title="设备接入中心时所用ID；取值范围：1-51200；默认值：4501";
	
		///////////////基础设置////////////////
		document.getElementById('epkID').title="同一链路下设备编号不应重复；取值范围：1-32；默认值：1";
		document.getElementById('intercomID').title="对讲设备标识，用于识别发起组呼、文本等业务的设备；取值范围：1-16776415；默认值：1";
		document.getElementById('power_div').title="设备当前功率；默认：低";
	
		document.getElementById('epkCode').title="相互通信的链路设备必须设置相同的链路色码，且不能与终端色码重复；取值范围：0-15；默认值：1";
		document.getElementById('termCode').title="相互通信的对讲设备必须设置相同的终端色码，且不能与链路色码重复；取值范围:0-15；默认值：5";
		document.getElementById('workMode').title="设备当前专网制式";
		document.getElementById('voiceCode').title="设备当前使用语音编码格式";
		document.getElementById('muteThreshold_div').title="门限越低，通话距离越远；默认：低";
		document.getElementById('micVolume_div').title="麦克风输入音量；默认：二级";
		document.getElementById('gsmVolume_div').title="GSM输出音量；默认：三级";
		document.getElementById('trumpetVolume_div').title="手咪输出音量；默认值：三级";
		document.getElementById('devCallTimeout').title="单次通话最长时间；取值范围：20-500；默认值：60";
		document.getElementById('neighbor_switch_status_div').title="设备邻点查询使能开关，默认：开启";
		document.getElementById('neighbor_period_div').title="设备邻点查询周期；默认：2";
		document.getElementById('neighbor_ai_switch_div').title="设备邻点信息主动上报使能开关，默认：关闭";
		document.getElementById('stop_transmit_switch_div').title="大功率禁止转发使能开关，默认：开启";
		document.getElementById('dev_language_switch').title="手咪语言切换开关，默认：中文";
		document.getElementById('boot_mode_switch').title="设备开机方式，默认：按键开机";
		document.getElementById('reboot_strategy_switch_div').title="设备启动策略，默认：快速启动";
		document.getElementById('alarm_switch_status_div').title="设备告警信息主动上报空口使能开关；默认：关闭";
		document.getElementById('alarm_count').title="紧急告警发送次数，值为0将关闭紧急警报功能；默认值：3";
		document.getElementById('alarm_show_switch_div').title="手咪显示紧急告警使能开关；默认；开启";
		document.getElementById('gsm_alarm_show_switch_div').title="GSM模块告警图标显示使能开关；默认：开启";
		document.getElementById('radio_alarm_show_switch_div').title="中转台告警图标显示使能开关；默认：开启";
	
		//////////////组呼设置////////////////
		document.getElementById('teamIdNow_div').title="当前组呼号";
	
		//////////////频率设置////////////////
		if(freq_channel ==0)
		{
			document.getElementById('frePointNow_div').title="";
		}
		else
		{
			if((freq_channel==1)||(freq_channel==2))
			{
				document.getElementById('frePointNow_div').title="设备当前频率";
			}
		}
	    
	
		if(mode == 0)
		{
			document.getElementById('old_user').title="请输入当前用户名";
			document.getElementById('new_user').title="请输入新的用户名，用户名长度5-20字符，只能输入数字与英文字母";
			document.getElementById('old_code').title="请输入当前密码";
			document.getElementById('new_code').title="请输入新的密码，密码长度5-20字符，只能输入数字与英文字母";
			document.getElementById('confirm_code').title="请再次输入新的密码";
		}
		else 
		{
			document.getElementById('blkListConf_div').title="黑名单使能开关，开启会屏蔽黑名单1-10中号码呼叫功能；默认：关闭";
			document.getElementById('whtListConf_div').title="白名单使能开关，开启会使白名单1-10中的号码开启呼叫功能；默认：关闭";
			document.getElementById('radioIP').title="中转台所用IP地址";
			document.getElementById('msgSlotTimePort1').title="中转台短信业务时隙1端口，默认值：30007";
			document.getElementById('msgSlotTimePort2').title="中转台短信业务时隙2端口，默认值：30008";
			document.getElementById('callCtrlSlotTimePort1').title="中转台呼叫控制时隙1端口，默认值：30009";
			document.getElementById('callCtrlSlotTimePort2').title="中转台呼叫控制时隙2端口，默认值：30010";
			document.getElementById('volumeSlotTimePort1').title="中转台语音业务时隙1端口，默认值：30012";
			document.getElementById('volumeSlotTimePort2').title="中转台语音业务时隙2端口，默认值：30014";
			document.getElementById('analogCallPort').title="中转台模拟呼叫控制端口，默认值：30015";
			document.getElementById('analogVolumePort').title="中转台模拟语音业务端口，默认值：30016";
			document.getElementById('radioID').title="中转台空口ID；取值范围：1-16776415，默认值：100";
			document.getElementById('interConctNum').title="中转台与中心互联所用组ID；取值范围：1-16776415，默认值：1000";
			document.getElementById('msgCtrlAdmID').title="短信控制管理员ID；取值范围：1-16776415，默认值：101";
			document.getElementById('msgCtrlFilter_div').title="短信过滤功能使能开关（开启时只接收来自管理员ID短信，关闭时可接收所有人员ID短信）；默认：关闭";
			
			//////////////////工程模式//////////////////////
			document.getElementById('gsm_call_interval').title="GSM主叫重呼间隔；取值范围：10-60，默认值：30";
			document.getElementById('gsm_reboot_time').title="GSM模块异常重启时间；取值范围：60-600，默认值：120";
			document.getElementById('vol_start_threshold_div').title="语音能量超过语音检测能量阈值持续时间达到门限值，认为检测出PTT_ON；默认值：80";
			document.getElementById('vol_delay').title="静音后话权释放时间；取值范围：1-30，默认值：3";
			document.getElementById('vol_energy_value').title="语音的能量门限；取值范围：1500-4000，默认值：2000";
			document.getElementById('vol_packet_time_div').title="单包语音的时长；默认值：60";
			document.getElementById('terminal_compatible_div').title="兼容MOTO终端使能开关；默认：开启，兼容MOTO终端模式";
			document.getElementById('work_mode_div').title="设备当前专网制式；默认：PDT常规";
			document.getElementById('voice_mode_div').title="设备当前使用的语音编码格式；默认值：NVOC";
			document.getElementById('linkTime_div').title="发射停止后业务挂断时间；默认值：0.5";
			document.getElementById('devOverTime').title="链路业务等待超时时间；取值范围：1-8；默认值：5";
			document.getElementById('termGpsOverTime').title="终端GPS上拉、摇晕、激活业务等待超时时间；取值范围：5-16；默认值：10";
			document.getElementById('close_transmit_rssi_threshold').title="接收到其它设备语音，若场强值大于该门限，则禁止转发；取值范围：-80~-40，默认值：-60";
			document.getElementById('resume_transmit_rssi_threshold').title="接收到其它设备语音，若场强值小于该门限，则恢复转发；取值范围：-90~-65，默认值：-70";
			document.getElementById('tempratue_alarm_start_threshold').title="大于该门限，触发温度告警；取值范围：0-100，默认值：85";
			document.getElementById('tempratue_alarm_close_threshold').title="小于该门限，取消温度告警；取值范围：0-100，默认值：80";
		}
	}
	else
	{
		///////////////网络参数////////////////
		document.getElementById('linkIP').title="Local IP Address";
		document.getElementById('mask').title="Subnet Mask";
		document.getElementById('gateWay').title="Gateway IP Address";
		document.getElementById('centerIP').title="Center IP Address";
		document.getElementById('centerID').title="ID used by the device to access the control center. Range 1-51200; Default: 4501";
	
		///////////////基础设置////////////////
		document.getElementById('epkID').title="It must be unique. Range: 1-32. Default: 1";
		document.getElementById('intercomID').title="ID of the radio. Range: 1-16776415. Default: 1";
		document.getElementById('power_div').title="Transmission power of the device. Default: Low";
	
		document.getElementById('epkCode').title="Links must have the same radio color code in order to communicate with each other. Range: 0-15; Default: 1";
		document.getElementById('termCode').title="Radios must have the same radio color code in order to communicate with each other. Range: 0-15; Default: 5";
		document.getElementById('workMode').title="Operation mode of the device";
		document.getElementById('voiceCode').title="Voice encoding format used by the device";
		document.getElementById('muteThreshold_div').title="Lower threshold means longer communication distance. Default: Low";
		document.getElementById('micVolume_div').title="Input volume of microphone. Default: Level 2";
		document.getElementById('gsmVolume_div').title="Output volume of GSM call. Default: Level 3";
		document.getElementById('trumpetVolume_div').title="Output volume of palm microphone. Default: Level 3";
		document.getElementById('devCallTimeout').title="Duration of a single voice transmission. Range: 20-500; Default: 60";
		document.getElementById('neighbor_switch_status_div').title="Whether to enable querying neighbor devices of the local device. Default: Enable";
		document.getElementById('neighbor_period_div').title="Interval for querying neighbor devices of the local device. Default: 2";
		document.getElementById('neighbor_ai_switch_div').title="Whether to enable the device to report information on neighbor devices. Default: Disable";
		document.getElementById('stop_transmit_switch_div').title="Whether to inhibit the signals will be prevented from being repeated if the RSSI is too high. Default: Enable";
		document.getElementById('dev_language_switch_div').title="Switches the interface language of the palm microphone. Default: Chinese";
		document.getElementById('boot_mode_switch_div').title="Boot method of the device. Default: Manual Boot";
		document.getElementById('reboot_strategy_switch_div').title="Device startup policy. Default: Quick Startup";
		document.getElementById('alarm_switch_status_div').title="Whether to enable the device to report alarms over the air. Default: Disable";
		document.getElementById('alarm_count').title="The number of emergency alarm transmissions. Emergency alarm will be disabled if it is set to 0. Default: 3";
		document.getElementById('alarm_show_switch_div').title="Whether to enable palm microphone to display emergency alarm. Default: Enable";
		document.getElementById('gsm_alarm_show_switch_div').title="Whether to enable palm microphone to display GSM module alarm. Default: Enable";
		document.getElementById('radio_alarm_show_switch_div').title="Whether to enable palm microphone to display repeater module alarm. Default: Enable";
	
		//////////////组呼设置////////////////
		document.getElementById('teamIdNow_div').title="Current group call number";
	
		//////////////频率设置////////////////
		if(freq_channel ==0)
		{
			document.getElementById('frePointNow_div').title="";
		}
		else
		{
			if((freq_channel==1)||(freq_channel==2))
			{
				document.getElementById('frePointNow_div').title="Frequency currently used by the device";
			}
		}
		if(mode == 0)
		{
			document.getElementById('old_user').title="Please enter the user name";
			document.getElementById('new_user').title="Please enter the new user name. The length is 5-20 characters. Enter only digits and English letters";
			document.getElementById('old_code').title="Please enter the password";
			document.getElementById('new_code').title="Please enter the new password. The length is 5-20 characters. Enter only digits and English letters";
			document.getElementById('confirm_code').title="Please enter the password again";
		}
		else 
		{
			document.getElementById('blkListConf_div').title="Whether to enable blacklist. Call from any number included on the blacklist 1-10 will be blocked. Default: Disable";
			document.getElementById('whtListConf_div').title="Whether to enable whitelist. Call from any number included on the whitelist 1-10 will be received. Default: Disable";
			document.getElementById('radioIP').title="IP address of the repeater";
			document.getElementById('msgSlotTimePort1').title="Port for repeater message service timeslot 1. Default: 30007";
			document.getElementById('msgSlotTimePort2').title="Port for repeater message service timeslot 2. Default: 30008";
			document.getElementById('callCtrlSlotTimePort1').title="Port for repeater call control timeslot 1. Default: 30009";
			document.getElementById('callCtrlSlotTimePort2').title="Port for repeater call control timeslot 2. Default: 30010";
			document.getElementById('volumeSlotTimePort1').title="Port for repeater voice service timeslot 1. Default: 30012";
			document.getElementById('volumeSlotTimePort2').title="Port for repeater voice service timeslot 2. Default: 30014";
			document.getElementById('analogCallPort').title="Port for repeater analog call control. Default: 30015";
			document.getElementById('analogVolumePort').title="Port for repeater analog call service. Default: 30016";
			document.getElementById('radioID').title="ID of the repeater. Range: 1-16776415; Default: 100";
			document.getElementById('interConctNum').title="Group ID used by the repeater to interconnect with the control center. Range: 1-16776415; Default: 1000";
			document.getElementById('msgCtrlAdmID').title="ID of the message manager. Range: 1-16776415; Default: 101";
			document.getElementById('msgCtrlFilter_div').title="Whether to enable message filter. If enabled, only messages from the message manager can be received. Default: Disable";
			
			//////////////////工程模式//////////////////////
			document.getElementById('gsm_call_interval').title="Interval between call setup attempts from the GSM contact. Range: 10-60; Default: 30";
			document.getElementById('gsm_reboot_time').title="Interval for the GSM module to recovery from fault. Range: 60-600; Default: 120";
			document.getElementById('vol_start_threshold_div').title="If the duration for which the energy of the detected voice exceeds the voice detection energy threshold, it means the speak right is obtained. Default: 80";
			document.getElementById('vol_delay').title="The duration after which the speak right will be taken away if no voice is detected. Range: 1-30; Default: 3";
			document.getElementById('vol_energy_value').title="If exceeded, it can be determined that the detected audio signal contains voice. Range: 1500-4000; Default: 2000";
			document.getElementById('vol_packet_time_div').title="Duration of a single voice packet. Default: 60";
			document.getElementById('terminal_compatible_div').title="Whether to enable the device to support Motorola radios. Default: Enable";
			document.getElementById('work_mode_div').title="Operation mode of the device. Default: PDT conventional";
			document.getElementById('voice_mode_div').title="Voice encoding format used by the device. Default: NVOC";
			document.getElementById('linkTime_div').title="The duration for which the link will be held after the transmission stops. Default: 0.5";
			document.getElementById('devOverTime').title="Timeout for the link service. Range: 1-8; Default: 5";
			document.getElementById('termGpsOverTime').title="Timeout for the radio service such as GPS polling, stunning, and reviving. Range: 5-16; Default: 10";
			document.getElementById('close_transmit_rssi_threshold').title="If RSSI of voice signals received from another device is larger than this threshold, the signals will be prevented from being repeated. Range: -40 to -80; Default: -60";
			document.getElementById('resume_transmit_rssi_threshold').title="If RSSI of voice signals received from another device is smaller than this threshold, the signal repeating will be resumed. Range: -90 to -65; Default: -70";
			document.getElementById('tempratue_alarm_start_threshold').title="If the temperature exceeds this threshold, an alarm will be triggered. Range: 0-100; Default: 85";
			document.getElementById('tempratue_alarm_close_threshold').title="If the temperature falls below this threshold, the temperature alarm will be canceled. Range: 0-100; Default: 80";
		}
	}
		
}

/**
 * @show_lable    英文登录，更改属性名
 * @author		wdz
 * @bug
 */
function  show_lable()
{
	var language_switch=$("#language_switch").val();
	var mode =$("#current_mode").val();
	var freq_channel=document.getElementById('freq_channel').value;
	
	if(language_switch==1)
	{
		$("#title_text").text("E-pack100 Standalone Network Management System - Configure");
		$("#h1_text").text("E-pack100 Standalone Network Management System ");
		$("#a1_text").html("<i class=\"icon iconfont icon-epack-9\"></i>Import");
		$("#a2_text").html("<i class=\"icon iconfont icon-epack-10\"></i>Export");
		$("#a3_text").html("<i class=\"icon iconfont icon-epack-18\"></i>Write");
		$("#a4_text").html("<i class=\"icon iconfont icon-epack-16\"></i>Read");
		$("#a5_text").html("<i class=\"icon iconfont icon-epack-17\"></i>Restart");
		
		$("#a6_text").html("<i class=\"icon iconfont icon-epack-1\"></i>Network &nbsp;&nbsp;Settings");
		$("#a7_text").html("<i class=\"icon iconfont icon-epack-2\"></i>&nbsp;Basic &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Settings");
		$("#a8_text").html("<i class=\"icon iconfont icon-epack-3\"></i>Group Call Settings");
		$("#a9_text").html("<i class=\"icon iconfont icon-epack-4\"></i>Frequency Settings");
		$("#a10_text").html("<i class=\"icon iconfont icon-epack-5\"></i>&nbsp;GSM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contacts");
		$("#a11_text").html("<i class=\"icon iconfont icon-epack-12\"></i>Upgrade &nbsp;&nbsp;&nbsp;Operate");
		$("#a17_text").html("<i class=\"icon iconfont icon-epack-6\"></i>Version &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Info");
		if(mode == 1)  //工程模式
		{
			$("#a12_text").html("<i class=\"icon iconfont icon-epack-15\"></i>Blacklist &nbsp;&nbsp;&nbsp;&nbsp;Settings");
			$("#a13_text").html("<i class=\"icon iconfont icon-epack-14\"></i>Whitelist &nbsp;&nbsp;&nbsp;Settings");
			$("#a14_text").html("<i class=\"icon iconfont icon-epack-13\"></i>Repeater &nbsp;&nbsp;&nbsp;Settings");
			$("#a15_text").html("<i class=\"icon iconfont icon-epack-20\"></i>Enginerer &nbsp;&nbsp;Mode");
		}
		else
		{
			$("#a16_text").html("<i class=\"icon iconfont icon-epack-21\"></i>User Management");
		}
		
		
		//////////////////////配置项/////////////////////////////
		//////////////////////网络参数/////////////////////////////
		$("#wlcs_text").text("Network Settings");
		$("#linkIP_text").text("Local IP Address");
		$("#mask_text").text("Subnet Mask ");
		$("#gateWay_text").text("Gateway IP Address");
		$("#centerIP_text").text("Center IP Address ");
		$("#centerID_text").text("Interconnection ID ");
		
		//////////////////////基础配置/////////////////////////////
		$("#jcsz_text").text("Basic Settings");
		$("#epkID_text").text("Local Device ID ");
		$("#intercomID_text").text("Radio ID");
		$("#power_text").text("TX Power");
		$("#power_1").text("Low");
		$("#power_2").text("Medium");
		$("#power_3").text("High");
		$("#epkCode_text").text("Link Color Code");
		$("#termCode_text").text("Radio Color Code");
		$("#workMode_text").text("Operation Mode");
		$("#voiceCode_text").text("Voice Codec ");
		$("#muteThreshold_text").text("Squelch Threshold ");
		$("#muteThreshold_1").text("Low");
		$("#muteThreshold_2").text("Medium");
		$("#muteThreshold_3").text("High");
		$("#micVolume_text").text("MIC Volume");
		$("#micVolume_1").text("Level 1");
		$("#micVolume_2").text("Level 2");
		$("#micVolume_3").text("Level 3");
		$("#gsmVolume_text").text("GSM Volume");
		$("#gsmVolume_1").text("Level 1");
		$("#gsmVolume_2").text("Level 2");
		$("#gsmVolume_3").text("Level 3");
		$("#gsmVolume_4").text("Level 4");
		$("#gsmVolume_5").text("Level 5");
		$("#trumpetVolume_text").text("Palm Microphone Volume");
		$("#trumpetVolume_1").text("Level 1");
		$("#trumpetVolume_2").text("Level 2");
		$("#trumpetVolume_3").text("Level 3");
		$("#trumpetVolume_4").text("Level 4");
		$("#trumpetVolume_5").text("Level 5");
		$("#devCallTimeout_text").text("Call Duration Limit");
		$("#neighbor_switch_status_text").text("Neighbor Query");
		$("#neighbor_switch_status_1").text("Disable");
		$("#neighbor_switch_status_2").text("Enable");
		$("#neighbor_period_text").text("Neighbor Query Interval");
		$("#neighbor_ai_switch_text").text("Report Neighbor to Center");
		$("#neighbor_ai_switch_1").text("Disable");
		$("#neighbor_ai_switch_2").text("Enable");
		$("#stop_transmit_switch_text").text("Smart Repeating");
		$("#stop_transmit_switch_1").text("Disable");
		$("#stop_transmit_switch_2").text("Enable");
		$("#dev_language_switch_text").text("Language Switch");
		$("#dev_language_switch_1").text("Chinese");
		$("#dev_language_switch_2").text("English")
		$("#boot_mode_switch_text").text("Boot Method");
		$("#boot_mode_switch_1").text("Manual Boot");
		$("#boot_mode_switch_2").text("Auto Boot upon Power-on")
		$("#reboot_strategy_switch_text").text("Startup Policy");
		$("#reboot_strategy_switch_1").text("Quick Startup");
		$("#reboot_strategy_switch_2").text("Full Startup");
		//////////////////////告警配置/////////////////////////////
		$("#alarm_text").text("Alarm Settings");
		$("#alarm_switch_status_text").text("Alarm Reporting");
		$("#alarm_switch_status_1").text("Disable");
		$("#alarm_switch_status_2").text("Enable");
		$("#alarm_count_text").text("Emergency Alarm Repetitions");
		$("#alarm_show_switch_text").text("Display Emergency Alarm");
		$("#alarm_show_switch_1").text("Disable");
		$("#alarm_show_switch_2").text("Enable");
		$("#gsm_alarm_show_switch_text").text("Display GSM Alarm");
		$("#gsm_alarm_show_switch_1").text("Disable");
		$("#gsm_alarm_show_switch_2").text("Enable");
		$("#radio_alarm_show_switch_text").text("Display Repeater Alarm");
		$("#radio_alarm_show_switch_1").text("Disable");
		$("#radio_alarm_show_switch_1").text("Enable");
		
		//////////////////////组呼配置/////////////////////////////
		$("#zhsz_text").text("Group Call Settings");
		$("#team1_text").text("Group 1");
		$("#team2_text").text("Group 2");
		$("#team3_text").text("Group 3");
		$("#team4_text").text("Group 4");
		$("#team5_text").text("Group 5");
		$("#team6_text").text("Group 6");
		$("#team7_text").text("Group 7");
		$("#team8_text").text("Group 8");
		$("#team9_text").text("Group 9");
		$("#team10_text").text("Group 10");
		$("#teamIdNow_text").text("Current Group");
		//////////////////////频率配置/////////////////////////////
		if(freq_channel ==1)
		{
			$("#plsz_text").text("Frequency 350 - 400 MHz");
		}
		else if(freq_channel ==2)
		{
			$("#plsz_text").text("Frequency 410 - 470 MHz");
		}
		else if(freq_channel ==0)
		{
			$("#plsz_text").text("The frequency band is invalid");
		}
		$("#frePoint1_text").text("Frequency 1");
		$("#frePoint2_text").text("Frequency 2");
		$("#frePoint3_text").text("Frequency 3");
		$("#frePoint4_text").text("Frequency 4");
		$("#frePoint5_text").text("Frequency 5");
		$("#frePoint6_text").text("Frequency 6");
		$("#frePoint7_text").text("Frequency 7");
		$("#frePoint8_text").text("Frequency 8");
		$("#frePoint9_text").text("Frequency 9");
		$("#frePoint10_text").text("Frequency 10");
		$("#frePointNow_text").text("Current Frequency");
		
		//////////////////////频率配置/////////////////////////////
		$("#gsmComm_text").text("Contacts");
		$("#gsmComm1_text").text("Contact 1");
		$("#gsmComm2_text").text("Contact 2");
		$("#gsmComm3_text").text("Contact 3");
		$("#gsmComm4_text").text("Contact 4");
		$("#gsmComm5_text").text("Contact 5");
		$("#gsmComm6_text").text("Contact 6");
		$("#gsmComm7_text").text("Contact 7");
		$("#gsmComm8_text").text("Contact 8");
		$("#gsmComm9_text").text("Contact 9");
		$("#gsmComm10_text").text("Contact 10");
		$("#gsmComm10_text").text("Contact 10");
		
		//////////////////////升级/////////////////////////////
		$("#update_text").text("Upgrade E-pack100");
		$("#update1_text").text("Upgrade File");
		$("#scan").text("Browse");
		$("#update2_text").text("Upgrade");
		
		//////////////////////版本信息/////////////////////////////
		$("#version_text").text("Version Info");
		$("#mac_text").text("Mac");
		$("#version1_text").text("E-pack100 Version");
		
		if(mode == 0)  //用户模式
		{
			//////////////////////用户管理/////////////////////////////
			$("#yhgl_text").text("User Management");
			$("#old_user_text").text("Old User Name");
			$("#new_user_text").text("New User Name");
			$("#old_code_text").text("Old Password");
			$("#new_code_text").text("New Password");
			$("#confirm_code_text").text("Confirm Password");
			$("#yhgl1_text").text("Submit");
		}
		else
		{
			//////////////////////黑名单/////////////////////////////
			$("#blk_text").text("Blacklist Settings");
			$("#blkListConf_text").text("Blacklist");
			$("#blkListConf_1").text("Disable");
			$("#blkListConf_2").text("Enable");
			$("#blkList1_text").text("Blacklist 1");
			$("#blkList2_text").text("Blacklist 2");
			$("#blkList3_text").text("Blacklist 3");
			$("#blkList4_text").text("Blacklist 4");
			$("#blkList5_text").text("Blacklist 5");
			$("#blkList6_text").text("Blacklist 6");
			$("#blkList7_text").text("Blacklist 7");
			$("#blkList8_text").text("Blacklist 8");
			$("#blkList9_text").text("Blacklist 9");
			$("#blkList10_text").text("Blacklist 10");
			
			//////////////////////白名单/////////////////////////////
			$("#whtList_text").text("Whitelist Settings");
			$("#whtListConf_text").text("Whitelist");
			$("#whtListConf_1").text("Disable");
			$("#whtListConf_2").text("Enable");
			$("#whtList1_text").text("Whitelist 1");
			$("#whtList2_text").text("Whitelist 2");
			$("#whtList3_text").text("Whitelist 3");
			$("#whtList4_text").text("Whitelist 4");
			$("#whtList5_text").text("Whitelist 5");
			$("#whtList6_text").text("Whitelist 6");
			$("#whtList7_text").text("Whitelist 7");
			$("#whtList8_text").text("Whitelist 8");
			$("#whtList9_text").text("Whitelist 9");
			$("#whtList10_text").text("Whitelist 10");
			
			//////////////////////中转台/////////////////////////////
			$("#radio_text").text("Repeater Settings");
			$("#radioIP_text").text("Repeater IP Address");
			$("#msgSlotTimePort1_text").text("Message Service Timeslot 1 Port");
			$("#msgSlotTimePort2_text").text("Message Service Timeslot 2 Port");
			$("#callCtrlSlotTimePort1_text").text("Call Control Timeslot 1 Port");
			$("#callCtrlSlotTimePort2_text").text("Call Control Timeslot 2 Port");
			$("#volumeSlotTimePort1_text").text("Voice Service Timeslot 1 Port");
			$("#volumeSlotTimePort2_text").text("Voice Service Timeslot 2 Port");
			$("#analogCallPort_text").text("Analog Call Control Port");
			$("#analogVolumePort_text").text("Analog Voice Service Port");
			$("#radioID_text").text("Repeater ID");
			$("#interConctNum_text").text("Repeater Group");
			$("#msgCtrlAdmID_text").text("Message Manager ID");
			$("#msgCtrlFilter_text").text("Message Filter");
			$("#msgCtrlFilter_1").text("Disable");
			$("#msgCtrlFilter_2").text("Enable");
			
			//////////////////////工程模式/////////////////////////////
			$("#gcms_text").text("Engineering Mode");
			$("#gsm_call_interval_text").text("GSM Recall Interval");
			$("#gsm_reboot_time_text").text("GSM Fault Recovery Interval");
			$("#vol_start_threshold_text").text("Voice Detection Start Threshold");
			$("#vol_delay_text").text("Speak Right Reset Delay");
			$("#vol_energy_value_text").text("Voice Detection Energy Threshold");
			$("#vol_packet_time_text").text("Voice Packet Length");
			$("#terminal_compatible_text").text("Support Motorola Radio");
			$("#terminal_compatible_1").text("Disable");
			$("#terminal_compatible_2").text("Enable");
			$("#work_mode_text").text("Operation Mode");
			$("#work_mode_1").text("PDT Conventional");
			$("#work_mode_2").text("DMR Conventional");
			$("#voice_mode_text").text("Voice Codec");
			$("#linkTime_text").text("Link Hold Time");
			$("#devOverTime_text").text("Link Service Hold Time");
			$("#termGpsOverTime_text").text("Radio Service Hold Time");
			$("#close_transmit_rssi_threshold_text").text("Repeating Stop RSSI Threshold");
			$("#resume_transmit_rssi_threshold_text").text("Repeating Recover RSSI Threshold");
			$("#tempratue_alarm_start_threshold_text").text("Temperature Alarm Threshold");
			$("#tempratue_alarm_close_threshold_text").text("Temperature Alarm Recovery Threshold");
		}
	}
	/*
	if(language_switch ==0)
	{
		$("#title_text").text("E-pack100 单机网络管理系统 - 配置");
		$("#h1_text").text("E-pack100 单机网络管理系统");
		$("#a1_text").html("<i class=\"icon iconfont icon-epack-9\"></i>导入");
		$("#a2_text").html("<i class=\"icon iconfont icon-epack-10\"></i>导出");
		$("#a3_text").html("<i class=\"icon iconfont icon-epack-18\"></i>写入");
		$("#a4_text").html("<i class=\"icon iconfont icon-epack-16\"></i>读取");
		$("#a5_text").html("<i class=\"icon iconfont icon-epack-17\"></i>重启");
		$("#a6_text").html("<i class=\"icon iconfont icon-epack-1\"></i>网络参数");
		$("#a7_text").html("<i class=\"icon iconfont icon-epack-2\"></i>基础设置");
		$("#a8_text").html("<i class=\"icon iconfont icon-epack-3\"></i>组呼设置");
		$("#a9_text").html("<i class=\"icon iconfont icon-epack-4\"></i>频率设置");
		$("#a10_text").html("<i class=\"icon iconfont icon-epack-5\"></i>GSM通讯录");
		$("#a11_text").html("<i class=\"icon iconfont icon-epack-12\"></i>&nbsp;升&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;级");
		$("#a17_text").html("<i class=\"icon iconfont icon-epack-6\"></i>版本信息");
		if(mode == 1)  //工程模式
		{
			$("#a12_text").html("<i class=\"icon iconfont icon-epack-15\"></i>黑名单设置");
			$("#a13_text").html("<i class=\"icon iconfont icon-epack-14\"></i>白名单设置");
			$("#a14_text").html("<i class=\"icon iconfont icon-epack-13\"></i>中转台设置");
			$("#a15_text").html("<i class=\"icon iconfont icon-epack-20\"></i>工程模式");
		}
		else
		{
			$("#a16_text").html("<i class=\"icon iconfont icon-epack-21\"></i>用户管理");
		}
	}
	else
	{
		$("#title_text").text("E-pack100");
		$("#h1_text").text("E-pack100 ");
		$("#a1_text").html("<i class=\"icon iconfont icon-epack-9\"></i>Import");
		$("#a2_text").html("<i class=\"icon iconfont icon-epack-10\"></i>Emport");
		$("#a3_text").html("<i class=\"icon iconfont icon-epack-18\"></i>Write");
		$("#a4_text").html("<i class=\"icon iconfont icon-epack-16\"></i>Read");
		$("#a5_text").html("<i class=\"icon iconfont icon-epack-17\"></i>Reboot");
		
		$("#a6_text").html("<i class=\"icon iconfont icon-epack-1\"></i>Network &nbsp;&nbsp;Settings");
		$("#a7_text").html("<i class=\"icon iconfont icon-epack-2\"></i>&nbsp;Basic &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Settings");
		$("#a8_text").html("<i class=\"icon iconfont icon-epack-3\"></i>Group Call Settings");
		$("#a9_text").html("<i class=\"icon iconfont icon-epack-4\"></i>Frequency Settings");
		$("#a10_text").html("<i class=\"icon iconfont icon-epack-5\"></i>&nbsp;GSM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contacts");
		$("#a11_text").html("<i class=\"icon iconfont icon-epack-12\"></i>Upgrade &nbsp;&nbsp;&nbsp;Operate");
		$("#a17_text").html("<i class=\"icon iconfont icon-epack-6\"></i>Version &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Info");
		if(mode == 1)  //工程模式
		{
			$("#a12_text").html("<i class=\"icon iconfont icon-epack-15\"></i>Blacklist &nbsp;&nbsp;&nbsp;&nbsp;Settings");
			$("#a13_text").html("<i class=\"icon iconfont icon-epack-14\"></i>Whitelist &nbsp;&nbsp;&nbsp;Settings");
			$("#a14_text").html("<i class=\"icon iconfont icon-epack-13\"></i>Repeater &nbsp;&nbsp;&nbsp;Settings");
			$("#a15_text").html("<i class=\"icon iconfont icon-epack-20\"></i>Enginerer &nbsp;&nbsp;Mode");
		}
		else
		{
			$("#a16_text").html("<i class=\"icon iconfont icon-epack-21\"></i>User Management");
		}
	}
	*/
}

/**
 * @brief    子网掩码相邻判断
 * @author		wdz
 * @bug
 */
function  validate_mask_continue()
{
	var mask=document.getElementById("mask").value;
	var num = 0;
	var aNum = mask.split(".");
	
	num += parseInt(aNum[0]) << 24;  
    num += parseInt(aNum[1]) << 16;  
    num += parseInt(aNum[2]) << 8;  
    num += parseInt(aNum[3]) << 0;  
    num = num >>> 0;//这个很关键，不然可能会出现负数的情况  
    console.log(""+num); 
	var arr = new Array();
    for (var i = 0,j=31;i<32; i++,j--) 
	{
		arr[j] = num % 2;
        num = parseInt(num / 2);
	}
	num=32;
	for(var i = 0; i<32; i++)
	{
		if(arr[i]==0)
		{
			num=i;
			break;
		}
	}
	if((32==num)||(31 == num))
	{
		return 1;
	}
	num++;
	for(; num<32; num++)
	{
		if(arr[num] == 1)
		{
			console.log("error");
			return 0;
		}
	}
	return 1;
	
}



/**
 * @brief    子网掩码与IP组合有效
 * @author		wdz
 * @bug
 */
function  validate_mask_and_ip()
{
	var linkIP=document.getElementById("linkIP").value;
	var mask=document.getElementById("mask").value;
	var mask_num = 0;
	var linkIP_num = 0;
	var mask_aNum = mask.split(".");
	var linkIP_aNum = linkIP.split(".");
	
	mask_num += parseInt(mask_aNum[0]) << 24;  
    mask_num += parseInt(mask_aNum[1]) << 16;  
    mask_num += parseInt(mask_aNum[2]) << 8;  
    mask_num += parseInt(mask_aNum[3]) << 0;  
    mask_num = mask_num >>> 0;//这个很关键，不然可能会出现负数的情况  
    console.log(""+mask_num); 
	
	
	linkIP_num += parseInt(linkIP_aNum[0]) << 24;  
    linkIP_num += parseInt(linkIP_aNum[1]) << 16;  
    linkIP_num += parseInt(linkIP_aNum[2]) << 8;  
    linkIP_num += parseInt(linkIP_aNum[3]) << 0;  
    linkIP_num = linkIP_num >>> 0;//这个很关键，不然可能会出现负数的情况  
    console.log(""+linkIP_num); 
	
	
	var mask_arr = new Array();
    for (var i = 0,j=31;i<32; i++,j--) 
	{
		mask_arr[j] = mask_num % 2;
        mask_num = parseInt(mask_num / 2);
	}
	mask_num=0;
	for(var i = 0; i<32; i++)
	{
		if(mask_arr[i]==0)
		{
			mask_num=i;
			break;
		}
	}
	
	var linkIP_arr = new Array();
    for (var i = 0,j=31;i<32; i++,j--) 
	{
		linkIP_arr[j] = linkIP_num % 2;
        linkIP_num = parseInt(linkIP_num / 2);
	}
	
	if(linkIP_arr[mask_num]==0)
	{
		var i=0;
		for(i=mask_num+1;i<32;i++)
		{
			if(linkIP_arr[i]==1)
			{
				break;
			}
		}
		if(i==32)
		{
			return 1;
		}
		return 0;
		
	}
	var i=0;
	for(i=mask_num+1;i<32;i++)
	{
		if(linkIP_arr[i]==0)
		{
			break;
		}
	}
	if(i==32)
	{
		return 2;
	}
	return 0;
	
}


/**
 * @brief    子网掩码与网关组合有效
 * @author		wdz
 * @bug
 */
function  validate_mask_and_gateway()
{
	var gateWay=document.getElementById("gateWay").value;
	var mask=document.getElementById("mask").value;
	var mask_num = 0;
	var gateWay_num = 0;
	var mask_aNum = mask.split(".");
	var gateWay_aNum = gateWay.split(".");
	
	mask_num += parseInt(mask_aNum[0]) << 24;  
    mask_num += parseInt(mask_aNum[1]) << 16;  
    mask_num += parseInt(mask_aNum[2]) << 8;  
    mask_num += parseInt(mask_aNum[3]) << 0;  
    mask_num = mask_num >>> 0;//这个很关键，不然可能会出现负数的情况  
    console.log(""+mask_num); 
	
	
	gateWay_num += parseInt(gateWay_aNum[0]) << 24;  
    gateWay_num += parseInt(gateWay_aNum[1]) << 16;  
    gateWay_num += parseInt(gateWay_aNum[2]) << 8;  
    gateWay_num += parseInt(gateWay_aNum[3]) << 0;  
    gateWay_num = gateWay_num >>> 0;//这个很关键，不然可能会出现负数的情况  
    console.log(""+gateWay_num); 
	
	
	var mask_arr = new Array();
    for (var i = 0,j=31;i<32; i++,j--) 
	{
		mask_arr[j] = mask_num % 2;
        mask_num = parseInt(mask_num / 2);
	}
	mask_num=0;
	for(var i = 0; i<32; i++)
	{
		if(mask_arr[i]==0)
		{
			mask_num=i;
			break;
		}
	}
	
	var gateWay_arr = new Array();
    for (var i = 0,j=31;i<32; i++,j--) 
	{
		gateWay_arr[j] = gateWay_num % 2;
        gateWay_num = parseInt(gateWay_num / 2);
	}
	
	if(gateWay_arr[mask_num]==0)
	{
		var i=0;
		for(i=mask_num+1;i<32;i++)
		{
			if(gateWay_arr[i]==1)
			{
				break;
			}
		}
		if(i==32)
		{
			return 1;
		}
		return 0;
		
	}
	var i=0;
	for(i=mask_num+1;i<32;i++)
	{
		if(gateWay_arr[i]==0)
		{
			break;
		}
	}
	if(i==32)
	{
		return 2;
	}
	return 0;
	
}
/**
 * @brief    子网掩码不能为0.0.0.0或者255.255.255.255
 * @author		wdz
 * @bug
 */
function  validate_mask_value()
{
	var mask=document.getElementById("mask").value;
	
	if("0.0.0.0" == mask)
	{
		return 1;
	}
	else if("255.255.255.255" == mask)
	{
		return 2;
	}
	return 0;

}
/**
 * @brief    写入操作之前验证组呼列表
 * @author		wdz
 * @bug
 */
function before_write_validate_groupNumber()
{
	var team_string='team';
	var i=0;
	var j=0;
	var next=0;
	var language_switch=$("#language_switch").val();
	
	for(i=1;i<11;i++)
	{
		team_string='team';
		team_string=team_string+i;
		if(validate_team_item(team_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		team_string='team';
		team_string=team_string+i;
		var base_value=document.getElementById(team_string).value;
		for(j=i+1;j<next;j++)
		{
			team_string='team';
			team_string=team_string+j;
			var current_value=document.getElementById(team_string).value;
			
			if(parseInt(base_value)==parseInt(current_value))
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		team_string='team';
		team_string=team_string+next;
		var current_value=document.getElementById(team_string).value;
		var validate_result=validate_team_item(team_string);
		
		if("" == current_value)
		{
			if(next==1)
			{
				document.getElementById('invalidate_groupNumber').value=next;
				if(0==language_switch)
				{
					alarm_tip_groupNumber_error(1,'确定','取消',"组1不能为空",'温馨提示');
				}
				else
				{
					alarm_tip_groupNumber_error(1,'OK','Cancel',"Group 1 cannot be empty",'Prompt');
				}
				return 0;
			}
			else if(next != 10)
			{
				for(i=next+1;i<11;i++)
				{
					team_string='team';
					team_string=team_string+i;
					if(document.getElementById(team_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_groupNumber').value=next;
					if(0==language_switch)
					{
						team_string="组";
						team_string=team_string+next;
						alarm_tip_groupNumber_error(1,'确定','取消','为保证组呼列表连续'+team_string+"不能为空",'温馨提示');
					}
					else
					{
						team_string="Group ";
						team_string=team_string+next;
						alarm_tip_groupNumber_error(1,'确定','取消','For continuity of the Group list, the '+team_string+" cannot be empty",'温馨提示');
					}
					
					return 0;
				}
			}	
		}
		else if(0==validate_result)
		{
			document.getElementById('invalidate_groupNumber').value=next;
			if(0==language_switch)
			{
				team_string="组";
				team_string=team_string+next;
				alarm_tip_groupNumber_error(1,'确定','取消',team_string+"输入格式错误",'温馨提示');
			}
			else
			{
				team_string="Group ";
				team_string=team_string+next;
				alarm_tip_groupNumber_error(1,'OK','Cancel',"The format of "+team_string+" is wrong",'Prompt');
			}
			
			return 0;
		}
		else
		{
			document.getElementById('invalidate_groupNumber').value=next;
			if(0==language_switch)
			{
				team_string="组";
				team_string=team_string+next;
				alarm_tip_groupNumber_error(1,'确定','取消',team_string+"输入值已存在",'温馨提示');
			}
			else
			{
				team_string="Group ";
				team_string=team_string+next;
				alarm_tip_groupNumber_error(1,'OK','Cancel',"The "+team_string+" already exists",'Prompt');
			}
			return 0;
		}
		for(i=1;i<next+1;i++)
		{
			team_string='team';
			team_string=team_string+i;		
			document.getElementById(team_string).disabled=false;			
		}
		return 1;
	}
	else
	{
		for(i=1;i<next;i++)
		{
			team_string='team';
			team_string=team_string+i;		
			document.getElementById(team_string).disabled=false;
					
		}
		return 1;
	}
}

/**
 * @brief    写入操作之前验证频率列表
 * @author		wdz
 * @bug
 */
function before_write_validate_freqlist()
{
	var freq_string='frePoint';
	var i=0;
	var j=0;
	var next=0;
	var language_switch=$("#language_switch").val();
	for(i=1;i<11;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		if(validate_freq_item(freq_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		var base_value=document.getElementById(freq_string).value;
		for(j=i+1;j<next;j++)
		{
			freq_string='frePoint';
			freq_string=freq_string+j;
			var current_value=document.getElementById(freq_string).value;
			
			if(parseFloat(base_value)==parseFloat(current_value))
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		freq_string='frePoint';
		freq_string=freq_string+next;
		var current_value=document.getElementById(freq_string).value;
		var validate_result=validate_freq_item(freq_string);
		
		if("" == current_value)
		{
			if(next==1)
			{
				document.getElementById('invalidate_freqNumber').value=next;
				if(0==language_switch)
				{
					alarm_tip_freqNumber_error(1,'确定','取消',"频率1不能为空",'温馨提示');
				}
				else
				{
					alarm_tip_freqNumber_error(1,'OK','Cancel',"Frequency 1 cannot be empty",'Prompt');
				}
				return 0;
			}
			else if(next !=10)
			{
				for(i=next+1;i<11;i++)
				{
					freq_string='frePoint';
					freq_string=freq_string+i;
					if(document.getElementById(freq_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_freqNumber').value=next;
					if(0==language_switch)
					{
						freq_string="频率";
						freq_string=freq_string+next;
						alarm_tip_freqNumber_error(1,'确定','取消','为保证频率列表连续'+freq_string+"不能为空",'温馨提示');
					}
					else
					{
						freq_string="Frequency ";
						freq_string=freq_string+next;
						alarm_tip_freqNumber_error(1,'确定','取消','For continuity of the Frequency list, the '+freq_string+" cannot be empty",'温馨提示');
					}
					
					return 0;
				}
			}	
		}
		else if(0==validate_result)
		{
			document.getElementById('invalidate_freqNumber').value=next;
			if(0==language_switch)
			{
				freq_string="频率";
				freq_string=freq_string+next;
				alarm_tip_freqNumber_error(1,'确定','取消',freq_string+"输入格式错误",'温馨提示');
			}
			else
			{
				freq_string="Frequency ";
				freq_string=freq_string+next;
				alarm_tip_freqNumber_error(1,'OK','Cancel',"The format of "+freq_string+" is wrong",'Prompt');
			}
			
			return 0;
			
		}
		else
		{
			document.getElementById('invalidate_freqNumber').value=next;
			if(0==language_switch)
			{
				freq_string="频率";
				freq_string=freq_string+next;
				alarm_tip_freqNumber_error(1,'确定','取消',freq_string+"输入值已存在",'温馨提示');
			}
			else
			{
				freq_string="Frequency ";
				freq_string=freq_string+next;
				alarm_tip_freqNumber_error(1,'OK','Cancel',"The "+freq_string+" already exists",'Prompt');
			}
			
			return 0;
		}
		for(i=1;i<next+1;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;		
			document.getElementById(freq_string).disabled=false;			
		}
		return 1;
	}
	else
	{
		for(i=1;i<next;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;		
			document.getElementById(freq_string).disabled=false;
		}
		return 1;
	}
}


/**
 * @brief    写入操作之前验证黑名单列表
 * @author		wdz
 * @bug
 */
function before_write_validate_blackNumber()
{
	var black_string='blkList';
	var i=0;
	var j=0;
	var next=0;
	var language_switch=$("#language_switch").val();
	for(i=1;i<11;i++)
	{
		black_string='blkList';
		black_string=black_string+i;
		if(validate_blklist_item(black_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		black_string='blkList';
		black_string=black_string+i;
		var base_value=document.getElementById(black_string).value;
		for(j=i+1;j<next;j++)
		{
			black_string='blkList';
			black_string=black_string+j;
			var current_value=document.getElementById(black_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	
	if(next !=11)  //存在无效、空  或者  重复项
	{
		black_string='blkList';
		black_string=black_string+next;
		var current_value=document.getElementById(black_string).value;
		var validate_result=validate_blklist_item(black_string);
		
		if("" == current_value)
		{
			if(next!=10)
			{
				for(i=next+1;i<11;i++)
				{
					black_string='blkList';
					black_string=black_string+i;
					if(document.getElementById(black_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_blackNumber').value=next;
					if(0==language_switch)
					{
						black_string="黑名单";
						black_string=black_string+next;
						alarm_tip_blackNumber_error(1,'确定','取消','为保证黑名单列表连续'+black_string+"不能为空",'温馨提示');
					}
					else
					{
						black_string="Blacklist ";
						black_string=black_string+next;
						alarm_tip_blackNumber_error(1,'确定','取消','For continuity of the Blacklist list, the '+black_string+" cannot be empty",'温馨提示');
					}
					return 0;
				}
			}
		}
		else if(0==validate_result)
		{
			document.getElementById('invalidate_blackNumber').value=next;
			if(0==language_switch)
			{
				black_string="黑名单";
				black_string=black_string+next;
				alarm_tip_blackNumber_error(1,'确定','取消',black_string+"输入格式错误",'温馨提示');
			}
			else
			{
				black_string="Blacklist ";
				black_string=black_string+next;
				alarm_tip_blackNumber_error(1,'OK','Cancel',"The format of "+black_string+" is wrong",'Prompt');
			}
			
			return 0;
		}
		else
		{
			document.getElementById('invalidate_blackNumber').value=next;
			if(0==language_switch)
			{
				black_string="黑名单";
				black_string=black_string+next;
				alarm_tip_blackNumber_error(1,'确定','取消',black_string+"输入值已存在",'温馨提示');
			}
			else
			{
				black_string="Blacklist ";
				black_string=black_string+next;
				alarm_tip_blackNumber_error(1,'OK','Cancel',"The "+black_string+" already exists",'Prompt');
			}
			
			return 0;
		}
		for(i=1;i<next+1;i++)
		{
			black_string='blkList';
			black_string=black_string+i;		
			document.getElementById(black_string).disabled=false;			
		}
		return 1;
	}
	else
	{
		for(i=1;i<next;i++)
		{
			black_string='blkList';
			black_string=black_string+i;		
			document.getElementById(black_string).disabled=false;			
		}
		return 1;
	}
}


/**
 * @brief    写入操作之前验证白名单列表
 * @author		wdz
 * @bug
 */

function before_write_validate_whiteNumber()
{
	var white_string='whtList';
	var i=0;
	var j=0;
	var next=0;
	var language_switch=$("#language_switch").val();
	
	for(i=1;i<11;i++)
	{
		white_string='whtList';
		white_string=white_string+i;
		if(validate_whtList_item(white_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		white_string='whtList';
		white_string=white_string+i;
		var base_value=document.getElementById(white_string).value;
		for(j=i+1;j<next;j++)
		{
			white_string='whtList';
			white_string=white_string+j;
			var current_value=document.getElementById(white_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	if(next !=11)  //存在无效、空  或者  重复项
	{
		white_string='whtList';
		white_string=white_string+next;
		var current_value=document.getElementById(white_string).value;
		var validate_result=validate_whtList_item(white_string);
		
		if(""==current_value)
		{
			if(next!=10)
			{
				for(i=next+1;i<11;i++)
				{
					white_string='whtList';
					white_string=white_string+i;
					if(document.getElementById(white_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_whiteNumber').value=next;
					if(0==language_switch)
					{
						white_string="白名单";
						white_string=white_string+next;
						alarm_tip_whiteNumber_error(1,'确定','取消','为保证白名单列表连续'+white_string+"不能为空",'温馨提示');
					}
					else
					{
						white_string="Whitelist ";
						white_string=white_string+next;
						alarm_tip_whiteNumber_error(1,'确定','取消','For continuity of the Whitelist list, the '+white_string+" cannot be empty",'温馨提示');
					}
					
					return 0;
				}
			}
		}
		else if(0==validate_result)
		{
			document.getElementById('invalidate_whiteNumber').value=next;
			if(0==language_switch)
			{
				white_string="白名单";
				white_string=white_string+next;
				alarm_tip_whiteNumber_error(1,'确定','取消',white_string+"输入格式错误",'温馨提示');
			}
			else
			{
				white_string="Whitelist ";
				white_string=white_string+next;
				alarm_tip_whiteNumber_error(1,'OK','Cancel',"The format of "+white_string+" is wrong",'Prompt');
			}
			
			return 0;
		}
		else
		{
			document.getElementById('invalidate_whiteNumber').value=next;
			if(0==language_switch)
			{
				white_string="白名单";
				white_string=white_string+num;
				alarm_tip_whiteNumber_error(1,'确定','取消',white_string+"输入值已存在",'温馨提示');
			}
			else
			{
				white_string="Whitelist ";
				white_string=white_string+num;
				alarm_tip_whiteNumber_error(1,'OK','Cancel',"The "+white_string+" already exists",'Prompt');
			}
			
			return 0;
		}
		for(i=1;i<next+1;i++)
		{
			white_string='whtList';
			white_string=white_string+i;		
			document.getElementById(white_string).disabled=false;			
		}
		return  1;
	}
	else
	{
		for(i=1;i<next;i++)
		{
			white_string='whtList';
			white_string=white_string+i;		
			document.getElementById(white_string).disabled=false;
		}
		return  1;
	}
}

/**
 * @brief    写入操作之前验证gsm列表
 * @author		wdz
 * @bug
 */
function before_write_validate_gsmNumber()
{
	var gsm_string='gsmComm';
	var i=0;
	var j=0;
	var next=0;
	var language_switch=$("#language_switch").val();
	for(i=1;i<11;i++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+i;
		if(validate_gsmComm_item(gsm_string)==0)
		{
			next=i;
			break;
		}
	}
	if(next==0)
	{
		next=11;
	}
	for(i=1;i<next;i++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+i;
		var base_value=document.getElementById(gsm_string).value;
		for(j=i+1;j<next;j++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+j;
			var current_value=document.getElementById(gsm_string).value;
			
			if(base_value==current_value)
			{
				next=j;
				break;
			}
		}
	}
	
	
	if(next !=11)  //存在无效、空  或者  重复项
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+next;
		var current_value=document.getElementById(gsm_string).value;
		var validate_result=validate_gsmComm_item(gsm_string);
		
		if("" == current_value)
		{
			if(next!=10)
			{
				for(i=next+1;i<11;i++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+i;
					if(document.getElementById(gsm_string).value!="")
					{
						break;
					}
				}
				if(i!=11)
				{
					document.getElementById('invalidate_gsmNumber').value=next;
					if(0==language_switch)
					{
						gsm_string="联系人";
						gsm_string=gsm_string+next;
						alarm_tip_gsmNumber_error(1,'确定','取消','为保证联系人列表连续'+gsm_string+"不能为空",'温馨提示');
					}
					else
					{
						gsm_string="Contact ";
						gsm_string=gsm_string+next;
						alarm_tip_gsmNumber_error(1,'确定','取消','For continuity of the Contact list, the '+gsm_string+" cannot be empty",'温馨提示');
					}
					
					return 0;
				}
			}
		}
		else if(0==validate_result)
		{
			document.getElementById('invalidate_gsmNumber').value=next;
			if(0==language_switch)
			{
				gsm_string="联系人";
				gsm_string=gsm_string+next;
				alarm_tip_gsmNumber_error(1,'确定','取消',gsm_string+"输入格式错误",'温馨提示');
			}
			else
			{
				gsm_string="Contact ";
				gsm_string=gsm_string+next;
				alarm_tip_gsmNumber_error(1,'OK','Cancel',"The format of "+gsm_string+" is wrong",'Prompt');
			}
			return 0;
		}
		else
		{
			document.getElementById('invalidate_gsmNumber').value=next;
			if(0==language_switch)
			{
				gsm_string="联系人";
				gsm_string=gsm_string+next;
				alarm_tip_gsmNumber_error(1,'确定','取消',gsm_string+"输入值已存在",'温馨提示');
			}
			else
			{
				gsm_string="Contact ";
				gsm_string=gsm_string+next;
				alarm_tip_gsmNumber_error(1,'OK','Cancel',"The "+gsm_string+" already exists",'Prompt');
			}
			
			return 0;
		}
		for(i=1;i<next+1;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;		
			document.getElementById(gsm_string).disabled=false;			
		}
		return  1;
	}
	else
	{
		for(i=1;i<next;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;		
			document.getElementById(gsm_string).disabled=false;			
		}
		return  1;
	}
}
