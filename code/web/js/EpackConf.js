


function alarm_tip(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}

function alarm_tip_log(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			window.location.href = "web_mgr.cgi";
																			//document.getElementById('again_login').click();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}


function alarm_tip_groupNumber_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
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
																			document.getElementById(team_string).focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}

function alarm_tip_groupNumber_repeat(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_groupNumber').value;
																			var team_string='team';
																			team_string=team_string+num;
																			document.getElementById(team_string).focus();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}
function alarm_tip_freqNumber_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
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
																			document.getElementById(freq_string).focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}


function alarm_tip_freqNumber_repeat(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_freqNumber').value;
																			var freq_string='frePoint';
																			freq_string=freq_string+num;
																			document.getElementById(freq_string).focus();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}


function alarm_tip_blackNumber_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
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
																			document.getElementById(black_string).focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['450px', '50%']		
				});
}

function alarm_tip_blackNumber_repeat(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_blackNumber').value;
																			var black_string='blkList';
																			black_string=black_string+num;
																			document.getElementById(black_string).focus();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}

function alarm_tip_whiteNumber_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
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
																			document.getElementById(white_string).focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['450px', '50%']		
				});
}

function alarm_tip_whiteNumber_repeat(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_whiteNumber').value;
																			var white_string='whtList';
																			white_string=white_string+num;
																			document.getElementById(white_string).focus();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}
function alarm_tip_gsmNumber_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
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
																			document.getElementById(gsm_string).focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}
function alarm_tip_gsmNumber_repeat(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			var num= document.getElementById('repeate_gsmNumber').value;
																			var gsm_string='gsmComm';
																			gsm_string=gsm_string+num;
																			document.getElementById(gsm_string).focus();
																			}},
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}

function alarm_tip_save_return(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, yes : function(index){ layer.close(index);document.getElementById("tmp_team").value=$("#teamIdNow").val();
																						document.getElementById("tmp_freq").value=$("#frePointNow").val();
																						if(document.getElementById("current_mode").value==0)
																						{
																							document.getElementById("set_operate_validate").click(); 
																						}
																						else
																						{
																							document.getElementById("set_operate").click(); 
																							layer.load('参数配置中，请等待。。。。');
																						}
																						
																					//	layer.load('参数配置中，请等待。。。。');
																					}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}

function alarm_tip_import_file(btns_num,option_info,arr){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, yes : function(index){ layer.close(index);var net_param_start=2;
																						var base_conf_start=8;
																						var team_conf_start=29;
																						var freq_conf_start=41;
																						var gsm_start=53;
																						var vertion_info_start=64;
																						setNetParam(arr[net_param_start+1],arr[net_param_start+2],arr[net_param_start+3],arr[net_param_start+4],arr[net_param_start+5]);
																						setBasConf(arr[base_conf_start+1],arr[base_conf_start+2],arr[base_conf_start+3],arr[base_conf_start+4],arr[base_conf_start+5],arr[base_conf_start+6],arr[base_conf_start+7],arr[base_conf_start+8],arr[base_conf_start+9],arr[base_conf_start+10],arr[base_conf_start+11],arr[base_conf_start+12],arr[base_conf_start+13],arr[base_conf_start+14]
																								,arr[base_conf_start+15],arr[base_conf_start+16],arr[base_conf_start+17],arr[base_conf_start+18],arr[base_conf_start+19],arr[base_conf_start+20]);
																						setTeamConf(arr[team_conf_start+1],arr[team_conf_start+2],arr[team_conf_start+3],arr[team_conf_start+4],arr[team_conf_start+5],arr[team_conf_start+6],arr[team_conf_start+7],arr[team_conf_start+8],arr[team_conf_start+9],arr[team_conf_start+10],arr[team_conf_start+11]);    
																						setFreConf(arr[freq_conf_start+1],arr[freq_conf_start+2],arr[freq_conf_start+3],arr[freq_conf_start+4],arr[freq_conf_start+5],arr[freq_conf_start+6],arr[freq_conf_start+7],arr[freq_conf_start+8],arr[freq_conf_start+9],arr[freq_conf_start+10],arr[freq_conf_start+11]);
																						setGsmConf(arr[gsm_start+1],arr[gsm_start+2],arr[gsm_start+3],arr[gsm_start+4],arr[gsm_start+5],arr[gsm_start+6],arr[gsm_start+7],arr[gsm_start+8],arr[gsm_start+9],arr[gsm_start+10]);
																						//setMidCovtConf(arr[midCovtConf_start+1],arr[midCovtConf_start+2],arr[midCovtConf_start+3],arr[midCovtConf_start+4],arr[midCovtConf_start+5],arr[midCovtConf_start+6],arr[midCovtConf_start+7],arr[midCovtConf_start+8],arr[midCovtConf_start+9],arr[midCovtConf_start+10],arr[midCovtConf_start+11],arr[midCovtConf_start+12],arr[midCovtConf_start+13]);
																						setVertionConf(arr[vertion_info_start+1],arr[vertion_info_start+2]);
																						alarm_tip(1,"导入文件成功")}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}
//////////////////////////加密///////////////////
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
//////////////////////////加密///////////////////
		
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

////////禁用后退按钮//////
function disable_back()
{
	history.pushState(null, null, document.URL);
    window.addEventListener('popstate', function () {
        history.pushState(null, null, document.URL);
        });
}
/*
////////刷新、关闭判断//////
function disable_flash()
{
	window.onbeforeunload = function() { 
	return "你确定要离开吗";
	};
}
*/

////////禁用快捷键//////
function disable_shortcut_key()
{      
    if ( (window.event.altKey)&&   
    (  (window.event.keyCode==37)|| //屏蔽 Alt+ 方向键 ←   
       (window.event.keyCode==39) )  ){ //屏蔽 Alt+ 方向键 →    
        event.returnValue=false;   
    }  
   if ( (event.keyCode==116)|| //屏蔽 F5 刷新键   
   　　(event.keyCode==112)|| //屏蔽 F1 刷新键   
    　(event.ctrlKey && event.keyCode==82))
	{ //Ctrl + R   
		event.keyCode=0;   
    　	event.returnValue=false;     
    }   
   if ((event.ctrlKey)&&(event.keyCode==78)) //屏蔽 Ctrl+n   
   {  
  　　 event.returnValue=false;   
   }  
   if ((event.shiftKey)&&(event.keyCode==121)) //屏蔽 shift+F10   
   {   
      event.returnValue=false;   
   }  
   if (window.event.srcElement.tagName == "A" && window.event.shiftKey)   
　 {  
   　　window.event.returnValue = false; //屏蔽 shift 加鼠标左键新开一网页   
　 }  
       
    if ((window.event.altKey)&&(window.event.keyCode==115))
	{ //屏蔽Alt+F4      
		return false;  
	}   
} 


    
function submit()
{
	if(validate_linkIp()==0)
	{
		showDiv(1);
		alarm_tip(1,'本机IP输入格式错误');
		return ;
    }
	
	if(validate_mask()==0)
	{
		showDiv(1);
		alarm_tip(1,'子网掩码输入格式错误');
		return;
    }
	if(validate_gateWay()==0)
	{
		showDiv(1);
		alarm_tip(1,'网关输入格式错误');
		return ;
    }
	if(validate_centerIP()==0)
	{
		showDiv(1);
		alarm_tip(1,'中心IP输入格式错误');
		return ;
    }
	if(validate_centerID()==0)
	{
		showDiv(1);
		alarm_tip(1,'互联ID输入格式错误');
		return ;
    }
	if(validate_epkID()==0)
	{
		showDiv(2);
		alarm_tip(1,'本机编号输入格式错误');
		return ;
    }
	if(validate_intercomID()==0)
	{
		showDiv(2);
		alarm_tip(1,'对讲ID输入格式错误');
		return ;
    }
	if(validate_power_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'发射功率输入格式错误');
		return ;
    }
	if(validate_epkCode()==0)
	{
		showDiv(2);
		alarm_tip(1,'链路色码输入格式错误');
		return ;
    }
	if(validate_termCode()==0)
	{
		showDiv(2);
		alarm_tip(1,'终端色码输入格式错误');
		return ;
    }
	if(validate_code()== false)
	{
		showDiv(2);
		alarm_tip(1,'终端色码应不同于链路色码');
		return ;
	}
	
	if(validate_muteThreshold_value()== 0)
	{
		showDiv(2);
		alarm_tip(1,'静噪门限输入格式错误');
		return ;
	}
	if(validate_micVolume_value()== 0)
	{
		showDiv(2);
		alarm_tip(1,'MIC音量输入格式错误');
		return ;
	}
	if(validate_gsmVolume_value()== 0)
	{
		showDiv(2);
		alarm_tip(1,'GSM音量输入格式错误');
		return ;
	}
	if(validate_trumpetVolume_value()== 0)
	{
		showDiv(2);
		alarm_tip(1,'手咪默认音量输入格式错误');
		return ;
	}
	/*
	if(validate_termDizzyOverTime()==0)
	{
		showDiv(2);
		alarm_tip('终端遥晕激活超时时间输入格式错误');
		return ;
    } */
	if(validate_devCallTimeout()==0)
	{
		showDiv(2);
		alarm_tip(1,'通话限时输入格式错误');
		return ;
    }
	if(validate_neighbor_switch_status_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'邻点查询使能输入格式错误');
		return ;
    }
	if(validate_neighbor_period_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'邻点周期输入格式错误');
		return ;
    }
	if(validate_neighbor_ai_switch_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'邻点上报使能输入格式错误');
		return ;
    }
	/*
	if(validate_neighbor_period()==0)
	{
		showDiv(2);
		alarm_tip(1,'邻点周期输入格式错误');
		return ;
    }
	*/
	/*
	if(validate_close_transmit_threshold()==0)
	{
		showDiv(2);
		alarm_tip('关闭发射场强门限输入格式错误');
		return ;
    }
	if(validate_resume_transmit_threshold()==0)
	{
		showDiv(2);
		alarm_tip('恢复发射场强门限输入格式错误');
		return ;
    }
	*/
	if(validate_alarm_switch_status_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'告警上报使能输入格式错误');
		return ;
    }
	
	if(validate_alarm_count()==0)
	{
		showDiv(2);
		alarm_tip(1,'紧急警报重复次数输入格式错误');
		return ;
    }
	if(validate_alarm_show_switch_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'紧急警报显示使能输入格式错误');
		return ;
    }
	if(validate_gsm_alarm_show_switch_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'GSM告警使能输入格式错误');
		return ;
    }
	if(validate_radio_alarm_show_switch_value()==0)
	{
		showDiv(2);
		alarm_tip(1,'中转台告警使能输入格式错误');
		return ;
    }
	var team_return=validate_team();
	if(team_return!=0)
	{	
		var team_num="组"+team_return;
		showDiv(3);
		alarm_tip(1,team_num+'输入格式错误');
		return ;
	}
	var team_return=check_groupNum_repeat();
	if(team_return!=0)
	{	
		var team_num="组"+team_return;
		showDiv(3);
		alarm_tip_groupNumber_repeat(1,team_num+"输入值已存在");
		return ;
	}
	if(validate_IdNow_in_team()==false)
	{
		showDiv(3);
		alarm_tip(1,'当前组不在组1~组10列表中');
		return ;
	}
	var freq_return=validate_freq();
	if(freq_return!=0)
	{
		var freq_num="频率"+freq_return;
		showDiv(4);
		alarm_tip(1,freq_num+'输入格式错误');
		return ;
	}
	var freq_return=check_freqNumber_repeat();
	if(freq_return!=0)
	{
		var freq_num="频率"+freq_return;
		showDiv(4);
		alarm_tip_freqNumber_repeat(1,freq_num+'输入值已存在');
		return ;
	}
	if(validate_freqNow_in_team()==false)
	{
		showDiv(4);
		alarm_tip(1,'当前频率不在频率1~频率10列表中');
		return ;
	}
	
	var gsmComm_return=validate_gsmComm();
	if(gsmComm_return!=0)
	{
		var gsmComm_num="联系人"+gsmComm_return;
		showDiv(5);
		alarm_tip(1,gsmComm_num+'输入格式错误');
		return ;
	}
	
	var gsmComm_return=check_gsmNumber_repeat();
	if(gsmComm_return!=0)
	{
		var gsmComm_num="联系人"+gsmComm_return;
		showDiv(5);
		alarm_tip_gsmNumber_repeat(1,gsmComm_num+'输入值已存在');
		return ;
	}
	
	////////////////工程模式//////////////////////
	if(document.getElementById("current_mode").value==1)
	{
		if(validate_blkListConf_value()==0)
		{
			showDiv(8);
			alarm_tip(1,'黑名单使能输入格式错误');
			return ;
		}
		
		var blklist_return=validate_blklist();
		if(blklist_return!=0)
		{
			var blklist_num="黑名单"+blklist_return;
			showDiv(8);
			alarm_tip(1,blklist_num+'输入格式错误');
			return ;
		}
	
		var blklist_return=check_blackNumber_repeat();
		if(blklist_return!=0)
		{
			var blklist_num="黑名单"+blklist_return;
			showDiv(8);
			alarm_tip_blackNumber_repeat(1,blklist_num+'输入值已存在');
			return ;
		}
		if(validate_whtListConf_value()==0)
		{
			showDiv(8);
			alarm_tip(1,'白名单使能输入格式错误');
			return ;
		}
		var whtList_return=validate_whtList();
		if(whtList_return!=0)
		{
			var whtList_num="白名单"+whtList_return;
			showDiv(9);
			alarm_tip(1,whtList_num+'输入格式错误');
			return ;
		}
	
		var whtList_return=check_whiteNumber_repeat();
		if(whtList_return!=0)
		{
			var whtList_num="白名单"+whtList_return;
			showDiv(9);
			alarm_tip_whiteNumber_repeat(1,whtList_num+'输入值已存在');
			return ;
		}
		if(validate_radioIP()==0)
		{
			showDiv(10);
			alarm_tip(1,'中转台IP输入格式错误');
			return ;
		}
		if(validate_radioID()==0)
		{
			showDiv(10);
			alarm_tip(1,'中转台ID输入格式错误');
			return ;
		}
		if(validate_interConctNum()==0)
		{
			showDiv(10);
			alarm_tip(1,'中转组输入格式错误');
			return ;
		}
		if(validate_msgCtrlAdmID()==0)
		{
			showDiv(10);
			alarm_tip(1,'短信控制管理员ID输入格式错误');
			return ;
		}
		if(validate_msgCtrlFilter_value()==0)
		{
			showDiv(10);
			alarm_tip(1,'短信控制过滤使能输入格式错误');
			return ;
		}
		if(validate_gsm_call_interval()==0)
		{
			showDiv(11);
			alarm_tip(1,'GSM主叫重呼间隔输入格式错误');
			return ;
		}
	
		if(validate_gsm_reboot_time()==0)
		{
			showDiv(11);
			alarm_tip(1,'GSM模块异常重启时间输入格式错误');
			return ;
		}
		if(validate_vol_start_threshold_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'语音检测启动门限输入格式错误');
			return ;
		}
		if(validate_vol_delay()==0)
		{
			showDiv(11);
			alarm_tip(1,'语音检测拖尾延时输入格式错误');
			return ;
		}
	
		if(validate_vol_energy_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'语音检测能量阈值输入格式错误');
			return ;
		}
		if(validate_vol_packet_time_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'语音包时长输入格式错误');
			return ;
		}
		if(validate_terminal_compatible_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'终端兼容模式输入格式错误');
			return ;
		}
		if(validate_work_mode_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'工作模式输入格式错误');
			return ;
		}
		if(validate_voice_mode_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'语音编码格式输入格式错误');
			return ;
		}
		if(validate_linkTime_value()==0)
		{
			showDiv(11);
			alarm_tip(1,'链路保持时间输入格式错误');
			return ;
		}
		if(validate_devOverTime()==0)
		{
			showDiv(11);
			alarm_tip(1,'链路业务保持时间输入格式错误');
			return ;
		}
		if(validate_termGpsOverTime()==0)
		{
			showDiv(11);
			alarm_tip(1,'终端业务保持时间输入格式错误');
			return ;
		}
		if(validate_close_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			alarm_tip(1,'关闭转发场强门限输入格式错误');
		return ;
		}
		if(validate_resume_transmit_rssi_threshold()==0)
		{
			showDiv(11);
			alarm_tip(1,'恢复转发场强门限输入格式错误');
			return ;
		}
		if(validate_tempratue_alarm_start_threshold()==0)
		{
			showDiv(11);
			alarm_tip(1,'温度告警门限输入格式错误');
			return ;
		}
		if(validate_tempratue_alarm_close_threshold()==0)
		{
			showDiv(11);
			alarm_tip(1,'温度告警恢复门限输入格式错误');
			return ;
		}
	}
	
	 alarm_tip_save_return(2,'参数配置成功会自动重启，是否配置');
}

function readParam_validate()
{
	
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	if(document.getElementById("current_mode").value==1)
	{
		document.getElementById("get_operate").click();
		layer.load('参数读取中，请等待。。。。');
	}
	else
	{
		document.getElementById("get_operate_validate").click();
	}
	
	//layer.load('参数读取中，请等待。。。。');
}
	
function readParam()
{
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("get_operate").click();
	layer.load('参数读取中，请等待。。。。');
}

function writeParam()
{
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("set_operate").click();
	layer.load('参数配置中，请等待。。。。');
}

function reboot_validate()
{
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	if(document.getElementById("current_mode").value==1)
	{
		document.getElementById("reboot_operate").click();
		layer.load('设备重启中，请等待。。。。');
	}
	else
	{
		document.getElementById("reboot_operate_validate").click();
	}
	
//	layer.load('设备重启中，请等待。。。。');
}
function reboot()
{
	convert_data_to_def();
	document.getElementById("tmp_team").value=$("#teamIdNow").val();
	document.getElementById("tmp_freq").value=$("#frePointNow").val();
	document.getElementById("reboot_operate").click();
	layer.load('设备重启中，请等待。。。。');
}

function  showTeam()
{
	
	for(var i=1;i<11;i++)
	{
		var team_string='team';
		team_string=team_string+i;
		
		if(validate_team_item(team_string)==0)
		{
			document.getElementById(team_string).value="";
			document.getElementById(team_string).disabled=false;
			if(i==1)
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415,不能为空";
			}
			else
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
			}
			
			i++;
			for(;i<11;i++)
			{   
				team_string='team';
				team_string=team_string+i;
				document.getElementById(team_string).value="";
				document.getElementById(team_string).disabled=true;
				document.getElementById(team_string).title="";
			}
			return ;
		}
		else
		{
			document.getElementById(team_string).disabled=false;
			if(i==1)
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415,不能为空";
			}
			else
			{
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
			}
		}
	}
	
}
function  showFreq()
{
	var freq_channel=document.getElementById('freq_channel').value;
	
	for(var i=1;i<11;i++)
	{
		var freq_string='frePoint';
		freq_string=freq_string+i;
		
		if(validate_freq_item(freq_string)==0)
		{
			document.getElementById(freq_string).value="";
			document.getElementById(freq_string).disabled=false;
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470,不能为空；输入格式：最多6位精度位数";
				}
				else
				{	
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
				}
			}
			
			
			i++;
			for(;i<11;i++)
			{   
				freq_string='frePoint';
				freq_string=freq_string+i;
				document.getElementById(freq_string).value="";
				document.getElementById(freq_string).disabled=true;
				document.getElementById(freq_string).title="";
			}
			return ;
		}
		else
		{
			document.getElementById(freq_string).disabled=false;
			if(freq_channel==1)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
				}
			}
			else if(freq_channel==2)
			{
				if(i==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400,不能为空；输入格式：最多6位精度位数";
				}
				else
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
				}
			}
			
		}
	}
	
}



function  showBlklist()
{
	
	for(var i=1;i<11;i++)
	{
		var Blklist_string='blkList';
		Blklist_string=Blklist_string+i;
		
		if(validate_blklist_item(Blklist_string)==0)
		{
			document.getElementById(Blklist_string).value="";
			document.getElementById(Blklist_string).disabled=false;
			
			document.getElementById(Blklist_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		
			i++;
			for(;i<11;i++)
			{   
				Blklist_string='blkList';
				Blklist_string=Blklist_string+i;
				document.getElementById(Blklist_string).value="";
				document.getElementById(Blklist_string).disabled=true;
				document.getElementById(Blklist_string).title="";
			}
			return ;
		}
		else
		{
			document.getElementById(Blklist_string).disabled=false;
			
			document.getElementById(Blklist_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
			
		}
	}
	
}
function  showWhtList()
{
	
	for(var i=1;i<11;i++)
	{
		var whtList_string='whtList';
		whtList_string=whtList_string+i;
	
		if(validate_whtList_item(whtList_string)==0)
		{
			document.getElementById(whtList_string).value="";
			document.getElementById(whtList_string).disabled=false;
			
			document.getElementById(whtList_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		
			i++;
			for(;i<11;i++)
			{   
				whtList_string='whtList';
				whtList_string=whtList_string+i;
				document.getElementById(whtList_string).value="";
				document.getElementById(whtList_string).disabled=true;
				document.getElementById(whtList_string).title="";
			}
			return ;
		}
		else
		{
			document.getElementById(whtList_string).disabled=false;
			
			document.getElementById(whtList_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
			
		}
	}
	
}

function  showgsmComm()
{
	
	for(var i=1;i<11;i++)
	{
		var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+i;
		
		if(validate_gsmComm_item(gsmComm_string)==0)
		{
			document.getElementById(gsmComm_string).value="";
			document.getElementById(gsmComm_string).disabled=false;
			
			document.getElementById(gsmComm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
		
			i++;
			for(;i<11;i++)
			{   
				gsmComm_string='gsmComm';
				gsmComm_string=gsmComm_string+i;
				document.getElementById(gsmComm_string).value="";
				document.getElementById(gsmComm_string).disabled=true;
				document.getElementById(gsmComm_string).title="";
			}
			return ;
		}
		else
		{
			document.getElementById(gsmComm_string).disabled=false;
			
			document.getElementById(gsmComm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
			
		}
	}
	
}
/////////////////////////导入////////////////////////////////////////////
function impot(){
   $("#uploadfile").click();
}


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
    var input = " <input type=\"file\" id=\"uploadfile\" name=\"uploadfile\" onchange=\"createUploadFile()\" accept=\"image/gif, image/jpeg, image/png\"/>";
    $("#myupload").append(input);
       
}

function setConfParam(confVal){
	/////解密/////
	var b = new Base64();  
	var confVal = b.decode(confVal); 
    var arr = confVal.split("=");//以=号作为分隔字符串
	var net_param_start=2;
	var base_conf_start=8;
	var team_conf_start=29;
	var freq_conf_start=41;
	var gsm_start=53;
	var vertion_info_start=64;
	var normal_num=64+2+1+1;//
	var blk_list_start=67;
	var white_list_start=79;
	var midCovtConf_start=91;
	var engieer_info_start=105;
	var engineer_num=105+16+1+1;
	var index=confVal.lastIndexOf("=");
	if(index == -1)
	{
		alarm_tip(1,'导入文件格式错误');
		return;
	}
	var sub_string=confVal.substring(0,index+1);
	var crc=crc32(sub_string);
	console.log(""+crc);
	console.log(""+arr.length+"aa"+normal_num+"bb"+engineer_num);
	if((crc != arr[arr.length-1])||(arr.length<normal_num))
	{
		alarm_tip(1,'导入文件格式错误');
		return;
	}
	if(document.getElementById('current_mode').value==1)
	{
		if(arr[1]==0)
		{
			alarm_tip_import_file(2,'工程模式是否导入用户模式文件',arr);
			return;
		}
		else if(arr.length!=engineer_num)
		{
			alarm_tip(1,'导入文件格式错误');
			return;
		}
		
	}
    setNetParam(arr[net_param_start+1],arr[net_param_start+2],arr[net_param_start+3],arr[net_param_start+4],arr[net_param_start+5]);
    setBasConf(arr[base_conf_start+1],arr[base_conf_start+2],arr[base_conf_start+3],arr[base_conf_start+4],arr[base_conf_start+5],arr[base_conf_start+6],arr[base_conf_start+7],arr[base_conf_start+8],arr[base_conf_start+9],arr[base_conf_start+10],arr[base_conf_start+11],arr[base_conf_start+12],arr[base_conf_start+13],arr[base_conf_start+14]
	           ,arr[base_conf_start+15],arr[base_conf_start+16],arr[base_conf_start+17],arr[base_conf_start+18],arr[base_conf_start+19],arr[base_conf_start+20]);
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
	alarm_tip(1,'导入文件成功');
}

//设置各配置模块参数方法
function setNetParam(param1,param2,param3,param4,param5){
    document.getElementById('linkIP').value = param1;
    document.getElementById('mask').value = param2;
    document.getElementById('gateWay').value = param3;
    document.getElementById('centerIP').value = param4;
    document.getElementById('centerID').value = param5;
}

function setBasConf(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15,param16,param17,param18,param19,param20){
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
	
	document.getElementById('alarm_switch_status').value = param16;
	document.getElementById('alarm_count').value = param17;
	document.getElementById('alarm_show_switch').value = param18;
	document.getElementById('gsm_alarm_show_switch').value = param19;
	document.getElementById('radio_alarm_show_switch').value = param20;
	
}

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
	
	showTeam();
	add_id_option();
	document.getElementById('teamIdNow').value = param1;
	
}

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
	
	showFreq();
	add_freq_option();
	document.getElementById('frePointNow').value = param1;
}

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
	
	showBlklist();
}

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
	
	showWhtList();
}

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
	
	showgsmComm();
}

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

function setVertionConf(param1,param2){
   document.getElementById('mac').value=param1;
   document.getElementById('version').value=param2;
}


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
//////////////////////工程模式//////////////////////
/////////////////////////导入////////////////////////////////////////////

function crc32 (str) {
/*
	function Utf8Encode(string) {
			string = string.replace(/\r\n/g,"\n");
			var utftext = "";
			for (var n = 0; n < string.length; n++)
			{
				var c = string.charCodeAt(n);

				if (c < 128) 
				{
					utftext += String.fromCharCode(c);
				}
				else if((c > 127) && (c < 2048)) 
				{
					utftext += String.fromCharCode((c >> 6) | 192);
					utftext += String.fromCharCode((c & 63) | 128);
				}
				else
				{
					utftext += String.fromCharCode((c >> 12) | 224);
					utftext += String.fromCharCode(((c >> 6) & 63) | 128);
					utftext += String.fromCharCode((c & 63) | 128);
				}

			}
        return utftext;
	};*/
 //   str = Utf8Encode(str);
//	console.log(str);
//	console.log(""+str.length);
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
function expt(){
    try {
        var isFileSaverSupported = !!new Blob;
    } catch (e) {  alert("该版本浏览器不支持导出！"); }
	
	var mode = getCurMode();
    var netParam = getNetParam();
    var basConf = getBasConf();
    var teamConf = getTeamConf();
    var freConf = getFreConf();
    var gsmConf = getGsmConf();
	var vertion_info=getVertionConf();
	
	
//	console.log(""+crc);
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
function getCurMode(){
    var mode =document.getElementById('current_mode').value;
    var retVal = "mode:="+mode+"=";
    return retVal;
}
function getNetParam(){
    var linkIP = document.getElementById('linkIP').value;
    var mask = document.getElementById('mask').value;
    var gateWay = document.getElementById('gateWay').value;
    var centerIP = document.getElementById('centerIP').value;
    var centerID = document.getElementById('centerID').value;
    var retVal = "web_conf:="+linkIP+"="+mask+"="+gateWay+"="+centerIP+"="+centerID+"=";
    return retVal;
}

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
	
	var alarm_switch_status=document.getElementById('alarm_switch_status').value;
	var alarm_count=document.getElementById('alarm_count').value;
	var alarm_show_switch=document.getElementById('alarm_show_switch').value;
	var gsm_alarm_show_switch=document.getElementById('gsm_alarm_show_switch').value;
	var radio_alarm_show_switch=document.getElementById('radio_alarm_show_switch').value ;
	
    var retVal = "bas_conf:="+epkID+"="+intercomID+"="+power+"="+epkCode+"="+termCode+"="+workMode+"="+voiceCode+"="+muteThreshold+"="+micVolume+"="+gsmVolume+"="+trumpetVolume+"="+
				devCallTimeout+"="+neighbor_switch_status+"="+neighbor_period+"="+neighbor_ai_switch+"="+alarm_switch_status+"="+alarm_count+"="+alarm_show_switch+"="+gsm_alarm_show_switch+"="+
				radio_alarm_show_switch+"=";
    return retVal;
}

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

function getVertionConf(){
	var mac = document.getElementById('mac').value;
    var epack_vertion = document.getElementById('version').value;
    var retVal = "ver_conf:="+mac+"="+epack_vertion+"=";
    return retVal;
}

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



////////////////////////////使能判断////////////////////////////////////////////

/*
function enableTeam(num){
    var inputId = "team"+num;
	
	if(validate_team_item(inputId)!=0)
	{
		    
			numAdd = num+1;
			inputIdNext = "team"+numAdd;
			document.getElementById(inputIdNext).disabled = false;
			
			
			$("#teamIdNow").empty();
			for(var i=1;i<numAdd;i++)
			{
				var exist=false;
				var team_string='team';
				team_string=team_string+i;
				var team_value=document.getElementById(team_string).value;
			
				$("#teamIdNow option").each(function() {
					tmp_value = $(this).val();
					if(tmp_value == team_value)
					{
						exist=true;
					}
				});
				if(exist == false)
				{
					$("#teamIdNow").append( "<option value="+team_value+">"+team_value+"</option>" );	
				}
			
			}
		add_id_option();
		
	}
    
}
*/
/*
function enableFrePoint(num){
    var inputId = "frePoint"+num;
	
	if(validate_freq_item(inputId)!=0)
	{
		
			numAdd = num+1;
			inputIdNext = "frePoint"+numAdd;
            document.getElementById(inputIdNext).disabled = false;
			
			
			$("#frePointNow").empty();
			for(var i=1;i<numAdd;i++)
			{
				var exist=false;
				var team_string='frePoint';
				team_string=team_string+i;
				var team_value=document.getElementById(team_string).value;
			
				$("#frePointNow option").each(function() {
					tmp_value = $(this).val();
					if(tmp_value == team_value)
					{
						exist=true;
					}
				});
				if(exist == false)
				{
					$("#frePointNow").append( "<option value="+team_value+">"+team_value+"</option>" );	
				}
			
			}
		add_freq_option();
	}
}
*/
/*

function enableBlkList(num){
    var inputId = "blkList"+num;
	var disable=document.getElementById(inputId).disabled;
	if(disable == false)
	{
		if(validate_blklist_item(inputId)!=0)
		{
			numAdd = num+1;
			inputIdNext = "blkList"+numAdd;
            document.getElementById(inputIdNext).disabled = false;
		}
	}
}

function enableWhtList(num){
    var inputId = "whtList"+num;
    var disable=document.getElementById(inputId).disabled;
	if(disable == false)
	{
		if(validate_whtList_item(inputId)!=0)
		{
			numAdd = num+1;
			inputIdNext = "whtList"+numAdd;
            document.getElementById(inputIdNext).disabled = false;
		}
	}
}

function enableGsmComm(num){
    var inputId = "gsmComm"+num;
    var disable=document.getElementById(inputId).disabled;
	if(disable == false)
	{
		if(validate_gsmComm_item(inputId)!=0)
		{
			numAdd = num+1;
			inputIdNext = "gsmComm"+numAdd;
            document.getElementById(inputIdNext).disabled = false;
		}
	}
}
*/
////////////////////////////使能判断////////////////////////////////////////////

function showOption(dev_power,linkTime,half_variance_threshold,mic_volume,gsm_volume, horn_volume,blacklist_switch,whitelist_switch,filter_switch,
					alarm_switch_status,neighbor_switch,alarm_show_switch,gsm_alarm_show_switch,radio_alarm_show_switch,neighbor_ai_switch,current_id,current_freq,
					vol_start_threshold,vol_packet_time,terminal_compatible,work_mode_2,vol_code_2,neighbor_period){
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

////////////////////////////操作完成提示////////////////////////////////////////////


function showTip(remind){
	if(remind== 1)
	{
		alarm_tip_log(1,'重启成功，请重新登录');
	}
	else if(remind== 2)
	{
		alarm_tip(1,'重启失败');
	}
	else if(remind== 3)
	{
		alarm_tip(1,'读取成功');
	}
	else if(remind== 4)
	{
		alarm_tip(1,'读取失败');
	}
	else if(remind== 5)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("reboot_operate").click();
		layer.load('配置成功，正在重启。。。。');
	}
	else if(remind== 6)
	{
		alarm_tip(1,'配置失败');
	}
	else if(remind== 7)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("reboot_operate").click();
		layer.load('升级成功，正在重启。。。。');
	}
	else if(remind== 8)
	{
		alarm_tip(1,'升级失败');
	}
	else if(remind== 9)
	{
		alarm_tip(1,'保存信息成功');
	}
	else if(remind== 10)
	{
		alarm_tip(1,'保存信息失败');
	}
	else if(remind== 11)
	{
		readParam();
	}
	else if(remind== 12)
	{
		alarm_tip_log(1,'用户信息有误，请重新登录');
	}
	else if(remind== 13)
	{
		writeParam();
	}
	else if(remind== 14)
	{
		reboot();
	}
	else if(remind== 15)
	{
		set_code();
	}
}






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

//点击升级按钮
function select_file()
{
	var tmp_name=document.getElementById("upfile").value;
	if(tmp_name == "")
	{
		alarm_tip('升级路径不能为空');	
	}
	else
	{
		var selectedFile = document.getElementById("path").files[0];//获取读取的File对象
        var name = selectedFile.name;//读取选中文件的文件名
		var size = selectedFile.size;//读取选中文件的大小
		console.log("文件名:"+name+"大小："+size);
		convert_data_to_def();
		if(name == "soc_system_dtb.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_dtb"); 
		}
		else  if(name =="bootloader.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_uboot");
		}
		else  if(name == "ramdisk_image.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_file");
		}
		else  if(name == "loadapp.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_loadapp");
		}
		else  if(name == "soc_system_rbf.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_rbf");
		}
		else  if(name == "zImage.bin")
		{
			alarm_tip_update__return(2,"升级成功会自动重启，是否升级","transmit_zimage");
		}
		else
		{
			alarm_tip('升级文件格式错误');
		}
	}
    
}
function alarm_tip_update__return(btns_num,option_info,key){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, yes : function(index){ layer.close(index);document.getElementById("tmp_team").value=$("#teamIdNow").val();
																						document.getElementById("tmp_freq").value=$("#frePointNow").val();
																						document.getElementById(key).click(); 
																						layer.load('升级中，请勿关闭网页。。。。');}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}


function update(flag)
{
	if(flag ==1)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_dtb_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==2)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_dtb").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==3)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_uboot_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==4)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_uboot").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==5)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_loadapp_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==6)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_loadapp").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==7)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_file_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==8)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_file").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==9)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_rbf_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==10)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_rbf").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==11)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("transmit_zimage_update").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
	else if(flag ==12)
	{
		document.getElementById("tmp_team").value=$("#teamIdNow").val();
		document.getElementById("tmp_freq").value=$("#frePointNow").val();
		document.getElementById("update_zimage").click();
		layer.load('升级中，请勿关闭网页。。。。');
	}
}
function path_click()
{
	$("#path").click();
}
function file_path()
{
	var name = document.getElementById("path").value;
	var array=name.split("\\");
	document.getElementById("upfile").value=array[array.length-1];
}
///////验证epack_ip////////////
function validate_linkIp()
{
	var linkIP = document.getElementById('linkIP').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(linkIP);
}
///////验证子网掩码////////////
function validate_mask()
{
	var mask = document.getElementById('mask').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(mask);
}
///////验证默认网关////////////
function validate_gateWay()
{
	var gateWay = document.getElementById('gateWay').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(gateWay);
}

///////验证中心IP////////////
function validate_centerIP()
{
	var centerIP = document.getElementById('centerIP').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(centerIP);
}
///////验证中转台IP////////////
function validate_radioIP()
{
	var radioIP = document.getElementById('radioIP').value;
	var ret=/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))[.]){3}(([0-1]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))$/;
	return ret.test(radioIP);
}
///////验证中心ID///////////
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

///////验证Epack_ID///////////
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
///////验证对讲ID///////////
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
///////验证Epack色码///////////
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
///////验证终端色码///////////
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

///////验证设备业务超时时间///////////
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
///////验证终端GPS上拉超时时间///////////
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

///////验证组///////////
function validate_team_item(team_num)
{
	var team1 = document.getElementById(team_num).value;
	var ret=/^[1-9]\d+$/;
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

function alarm_tip_groupNumber(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, 
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
														document.getElementById('team1').focus();
													}
													else
													{
														add_id_option();
													}
													
												}
												else
												{
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
														document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
														break;
													}
													else
													{
														document.getElementById(team_string).disabled=false;
														document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
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
												document.getElementById(team_string).focus();
											}
						 },
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}


function alarm_tip_freqNumber(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, 
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
														document.getElementById('frePoint1').focus();
													}
													else
													{
														add_freq_option();
													}
													
												}
												else
												{
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
														if(freq_channel ==1)
														{
															document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
														}
														else if(freq_channel == 2)
														{
															document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
														}
														break;
													}
													else
													{
														document.getElementById(freq_string).disabled=false;
														if(freq_channel ==1)
														{
															document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
														}
														else if(freq_channel == 2)
														{
															document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
														}
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
												document.getElementById(freq_string).focus();
											}
						 },
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}



function alarm_tip_blackNumber(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, 
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
													document.getElementById(black_string).focus();
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
														document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
														break;
													}
													else
													{
														document.getElementById(black_string).disabled=false;
														document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
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
												document.getElementById(black_string).focus();
											}
						 },
				title : ['温馨提示',true],
				offset: ['450px', '50%']		
				});
}


function alarm_tip_whiteNumber(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, 
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
													document.getElementById(white_string).focus();
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
														document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
														break;
													}
													else
													{
														document.getElementById(white_string).disabled=false;
														document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
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
												document.getElementById(white_string).focus();
											}
						 },
				title : ['温馨提示',true],
				offset: ['450px', '50%']		
				});
}


function alarm_tip_gsmNumber(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info, 
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
													document.getElementById(gsm_string).focus();
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
														document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
														break;
													}
													else
													{
														document.getElementById(gsm_string).disabled=false;
														document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
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
												document.getElementById(gsm_string).focus();
											}
						 },
				title : ['温馨提示',true],
				offset: ['400px', '50%']		
				});
}


function groupNumber_focus(num)
{
	
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
function freqNumber_focus(num)
{
	
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

function blackNumber_focus(num)
{
	
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

function whiteNumber_focus(num)
{
	
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

function gsmNumber_focus(num)
{
	
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

function groupNumber_blur(num)
{
	var team_string='team';
	var i=0;
	var j=0;
	var next=0;
	var exist=false;
	team_string=team_string+num;
	var current_value=document.getElementById(team_string).value;
	var validate_result=validate_team_item(team_string);
	
	if("" == current_value)
	{
		if(num==1)
		{
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			alarm_tip_groupNumber_return(1,"组1不能为空");
			return ;
		}
		team_string='team';
		next=num+1;
		team_string=team_string+next;
		
		if(0!=validate_team_item(team_string))
		{
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			team_string="组";
			team_string=team_string+num;
			alarm_tip_groupNumber(2,team_string+'输入为空，为保证组呼列表连续是否清空后续列表');
		}
		else
		{
			add_id_option();
		}
		
		
	}
	else if(0==validate_result)
	{
		team_string='team';
		next=num+1;
		team_string=team_string+next;
		
		if(0!=validate_team_item(team_string))
		{
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			team_string="组";
			team_string=team_string+num;
			alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
		}
		else
		{
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			team_string="组";
			team_string=team_string+num;
			alarm_tip_groupNumber_return(1,team_string+"输入格式错误");
		}
	}
	else
	{
		for(i=1;i<11;i++)
		{
			if(i!=num)
			{
				team_string='team';
				team_string=team_string+i;
				if(validate_gsmComm_item(team_string)==0)
				{
					break;
				}
				else 
				{
					if(parseInt(document.getElementById(team_string).value)==parseInt(current_value))
					{
						document.getElementById('invalidate_groupNumber').value=num;
						exist=true;
						for(j=1;j<11;j++)
						{
							team_string='team';
							team_string=team_string+j;
							document.getElementById(team_string).readOnly=true;
						}
						$("#teamIdNow").empty();
						team_string="组";
						team_string=team_string+num;
						alarm_tip_groupNumber_return(1,team_string+"输入值已存在");
						break;
					}
				}
			}
		}
		
		next=num+1;
		for(i=next;i<11;i++)
		{
			team_string='team';
			team_string=team_string+i;
			if(validate_gsmComm_item(team_string)==0)
			{
				document.getElementById(team_string).disabled=false;
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
				break;
			}
			else
			{
				document.getElementById(team_string).disabled=false;
				document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
			}
													
		}
		if(exist == false)
		{
			add_id_option();
		}
		
		 
	}
	for(i=1;i<num;i++)
	{
		team_string='team';
		team_string=team_string+i;		
		document.getElementById(team_string).disabled=false;
		if(i==1)
		{
			document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415,不能为空";
		}
		else
		{
			document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
		}			
	}
}


function freqNumber_blur(num)
{
	var freq_string='frePoint';
	var i=0;
	var j=0;
	var next=0;
	var exist=false;
	freq_string=freq_string+num;
	var current_value=document.getElementById(freq_string).value;
	var validate_result=validate_freq_item(freq_string);
	var freq_channel=document.getElementById('freq_channel').value;
	if("" == current_value)
	{
		if(num==1)
		{
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			alarm_tip_freqNumber_return(1,"频率1不能为空");
			return ;
		}
		freq_string='frePoint';
		next=num+1;
		freq_string=freq_string+next;
		
		if(0!=validate_freq_item(freq_string))
		{
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			freq_string="频率";
			freq_string=freq_string+num;
			alarm_tip_freqNumber(2,freq_string+'输入为空，为保证频率列表连续是否清空后续列表');
		}
		else
		{
			add_freq_option();
		}
		
		
	}
	else if(0==validate_result)
	{
		freq_string='frePoint';
		next=num+1;
		freq_string=freq_string+next;
		
		if(0!=validate_freq_item(freq_string))
		{
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			freq_string="频率";
			freq_string=freq_string+num;
			alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
		}
		else
		{
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			freq_string="频率";
			freq_string=freq_string+num;
			alarm_tip_freqNumber_return(1,freq_string+"输入格式错误");
		}
	}
	else
	{
		for(i=1;i<11;i++)
		{
			if(i!=num)
			{
				freq_string='frePoint';
				freq_string=freq_string+i;
				if(validate_freq_item(freq_string)==0)
				{
					break;
				}
				else 
				{
					if(parseFloat(document.getElementById(freq_string).value)==parseFloat(current_value))
					{
						document.getElementById('invalidate_freqNumber').value=num;
						exist=true;
						for(j=1;j<11;j++)
						{
							freq_string='frePoint';
							freq_string=freq_string+j;
							document.getElementById(freq_string).readOnly=true;
						}
						$("#frePointNow").empty();
						freq_string="频率";
						freq_string=freq_string+num;
						alarm_tip_freqNumber_return(1,freq_string+"输入值已存在");
						break;
					}
				}
			}
		}
		
		next=num+1;
		for(i=next;i<11;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;
			if(validate_freq_item(freq_string)==0)
			{
				document.getElementById(freq_string).disabled=false;
				if(freq_channel ==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
				}
				else if(freq_channel == 2)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
				}
				break;
			}
			else
			{
				document.getElementById(freq_string).disabled=false;
				if(freq_channel ==1)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
				}
				else if(freq_channel == 2)
				{
					document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
				}
			}
													
		}
		if(exist == false)
		{
			add_freq_option();
		}
		
		 
	}
	for(i=1;i<num;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;		
		document.getElementById(freq_string).disabled=false;
		if(freq_channel ==1)
		{
			if(i==1)
			{
				document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400,不能为空；输入格式：最多6位精度位数";
			}
			else
			{
				document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
			}
		}
		else if(freq_channel == 2)
		{
			if(i==1)
			{
				document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470,不能为空；输入格式：最多6位精度位数";
			}
			else
			{
				document.getElementById(freq_string).title="设备可用频率列表；取值范围：410~470；输入格式：最多6位精度位数";
			}
		}
					
	}
}


function blackNumber_blur(num)
{
	var black_string='blkList';
	var i=0;
	var j=0;
	var next=0;
	black_string=black_string+num;
	var current_value=document.getElementById(black_string).value;
	var validate_result=validate_blklist_item(black_string);
	
	if("" == current_value)
	{
		black_string='blkList';
		next=num+1;
		black_string=black_string+next;
		
		if(0!=validate_blklist_item(black_string))
		{
			document.getElementById('invalidate_blackNumber').value=num;
			for(j=1;j<11;j++)
			{
				black_string='blkList';
				black_string=black_string+j;
				document.getElementById(black_string).readOnly=true;
			}
			black_string="黑名单";
			black_string=black_string+num;
			alarm_tip_blackNumber(2,black_string+'输入为空，为保证黑名单列表连续是否清空后续列表');
		}	
	}
	else if(0==validate_result)
	{
		black_string='blkList';
		next=num+1;
		black_string=black_string+next;
		
		if(0!=validate_blklist_item(black_string))
		{
			document.getElementById('invalidate_blackNumber').value=num;
			for(j=1;j<11;j++)
			{
				black_string='blkList';
				black_string=black_string+j;
				document.getElementById(black_string).readOnly=true;
			}
			black_string="黑名单";
			black_string=black_string+num;
			alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
		}
		else
		{
			document.getElementById('invalidate_blackNumber').value=num;
			for(j=1;j<11;j++)
			{
				black_string='blkList';
				black_string=black_string+j;
				document.getElementById(black_string).readOnly=true;
			}
			black_string="黑名单";
			black_string=black_string+num;
			alarm_tip_blackNumber_return(1,black_string+"输入格式错误");
		}
	}
	else
	{
		for(i=1;i<11;i++)
		{
			if(i!=num)
			{
				black_string='blkList';
				black_string=black_string+i;
				if(validate_blklist_item(black_string)==0)
				{
					break;
				}
				else 
				{
					if(document.getElementById(black_string).value==current_value)
					{
						document.getElementById('invalidate_blackNumber').value=num;
						for(j=1;j<11;j++)
						{
							black_string='blkList';
							black_string=black_string+j;
							document.getElementById(black_string).readOnly=true;
						}
						black_string="黑名单";
						black_string=black_string+num;
						alarm_tip_blackNumber_return(1,black_string+"输入值已存在");
						break;
					}
				}
			}
		}
		
		next=num+1;
		for(i=next;i<11;i++)
		{
			black_string='blkList';
			black_string=black_string+i;
			if(validate_blklist_item(black_string)==0)
			{
				document.getElementById(black_string).disabled=false;
				document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
				break;
			}
			else
			{
				document.getElementById(black_string).disabled=false;
				document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
			}
													
		}	 
	}
	for(i=1;i<num;i++)
	{
		black_string='blkList';
		black_string=black_string+i;		
		document.getElementById(black_string).disabled=false;
		document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
					
	}
}


function whiteNumber_blur(num)
{
	var white_string='whtList';
	var i=0;
	var j=0;
	var next=0;
	white_string=white_string+num;
	var current_value=document.getElementById(white_string).value;
	var validate_result=validate_whtList_item(white_string);
	
	if("" == current_value)
	{
		white_string='whtList';
		next=num+1;
		white_string=white_string+next;
		
		if(0!=validate_whtList_item(white_string))
		{
			document.getElementById('invalidate_whiteNumber').value=num;
			for(j=1;j<11;j++)
			{
				white_string='whtList';
				white_string=white_string+j;
				document.getElementById(white_string).readOnly=true;
			}
			white_string="白名单";
			white_string=white_string+num;
			alarm_tip_whiteNumber(2,white_string+'输入为空，为保证白名单列表连续是否清空后续列表');
		}	
	}
	else if(0==validate_result)
	{
		white_string='whtList';
		next=num+1;
		white_string=white_string+next;
		
		if(0!=validate_whtList_item(white_string))
		{
			document.getElementById('invalidate_whiteNumber').value=num;
			for(j=1;j<11;j++)
			{
				white_string='whtList';
				white_string=white_string+j;
				document.getElementById(white_string).readOnly=true;
			}
			white_string="白名单";
			white_string=white_string+num;
			alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
		}
		else
		{
			document.getElementById('invalidate_whiteNumber').value=num;
			for(j=1;j<11;j++)
			{
				white_string='whtList';
				white_string=white_string+j;
				document.getElementById(white_string).readOnly=true;
			}
			white_string="白名单";
			white_string=white_string+num;
			alarm_tip_whiteNumber_return(1,white_string+"输入格式错误");
		}
	}
	else
	{
		for(i=1;i<11;i++)
		{
			if(i!=num)
			{
				white_string='whtList';
				white_string=white_string+i;
				if(validate_whtList_item(white_string)==0)
				{
					break;
				}
				else 
				{
					if(document.getElementById(white_string).value==current_value)
					{
						document.getElementById('invalidate_whiteNumber').value=num;
						for(j=1;j<11;j++)
						{
							white_string='whtList';
							white_string=white_string+j;
							document.getElementById(white_string).readOnly=true;
						}
						white_string="白名单";
						white_string=white_string+num;
						alarm_tip_whiteNumber_return(1,white_string+"输入值已存在");
						break;
					}
				}
			}
		}
		
		next=num+1;
		for(i=next;i<11;i++)
		{
			white_string='whtList';
			white_string=white_string+i;
			if(validate_whtList_item(white_string)==0)
			{
				document.getElementById(white_string).disabled=false;
				document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
				break;
			}
			else
			{
				document.getElementById(white_string).disabled=false;
				document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
			}
													
		}	 
	}
	for(i=1;i<num;i++)
	{
		white_string='whtList';
		white_string=white_string+i;		
		document.getElementById(white_string).disabled=false;
		document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
					
	}
}


function gsmNumber_blur(num)
{
	var gsm_string='gsmComm';
	var i=0;
	var j=0;
	var next=0;
	gsm_string=gsm_string+num;
	var current_value=document.getElementById(gsm_string).value;
	var validate_result=validate_gsmComm_item(gsm_string);
	
	if("" == current_value)
	{
		gsm_string='gsmComm';
		next=num+1;
		gsm_string=gsm_string+next;
		
		if(0!=validate_gsmComm_item(gsm_string))
		{
			document.getElementById('invalidate_gsmNumber').value=num;
			for(j=1;j<11;j++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+j;
				document.getElementById(gsm_string).readOnly=true;
			}
			gsm_string="联系人";
			gsm_string=gsm_string+num;
			alarm_tip_gsmNumber(2,gsm_string+'输入为空，为保证联系人列表连续是否清空后续列表');
		}	
	}
	else if(0==validate_result)
	{
		gsm_string='gsmComm';
		next=num+1;
		gsm_string=gsm_string+next;
		
		if(0!=validate_gsmComm_item(gsm_string))
		{
			document.getElementById('invalidate_gsmNumber').value=num;
			for(j=1;j<11;j++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+j;
				document.getElementById(gsm_string).readOnly=true;
			}
			gsm_string="联系人";
			gsm_string=gsm_string+num;
			alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
		}
		else
		{
			document.getElementById('invalidate_gsmNumber').value=num;
			for(j=1;j<11;j++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+j;
				document.getElementById(gsm_string).readOnly=true;
			}
			gsm_string="联系人";
			gsm_string=gsm_string+num;
			alarm_tip_gsmNumber_return(1,gsm_string+"输入格式错误");
		}
	}
	else
	{
		for(i=1;i<11;i++)
		{
			if(i!=num)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+i;
				if(validate_gsmComm_item(gsm_string)==0)
				{
					break;
				}
				else 
				{
					if(document.getElementById(gsm_string).value==current_value)
					{
						document.getElementById('invalidate_gsmNumber').value=num;
						for(j=1;j<11;j++)
						{
							gsm_string='gsmComm';
							gsm_string=gsm_string+j;
							document.getElementById(gsm_string).readOnly=true;
						}
						gsm_string="联系人";
						gsm_string=gsm_string+num;
						alarm_tip_gsmNumber_return(1,gsm_string+"输入值已存在");
						break;
					}
				}
			}
		}
		
		next=num+1;
		for(i=next;i<11;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;
			if(validate_gsmComm_item(gsm_string)==0)
			{
				document.getElementById(gsm_string).disabled=false;
				document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
				break;
			}
			else
			{
				document.getElementById(gsm_string).disabled=false;
				document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
			}
													
		}	 
	}
	for(i=1;i<num;i++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+i;		
		document.getElementById(gsm_string).disabled=false;
		document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，如+1234或1234";
					
	}
}

function check_groupNum_repeat()
{
	var team_string='team';
	var invalidate_num=0;
	var exist_num=0;
	var i=0;
	var j=0;
	var validate_result=0;
	var exist_flag=false;
	
	for(i=1;i<11;i++)
	{
		team_string='team';
		team_string=team_string+i;
		validate_result=validate_team_item(team_string);
		if(0==validate_result)
		{
			invalidate_num=i;
			break;
		}
	}
	if(i==11)
	{
		invalidate_num=11;
	}
	for(i=1;i<invalidate_num;i++)
	{
		team_string='team';
		team_string=team_string+i;
		var base_value=document.getElementById(team_string).value;
		
		for(j=i+1;j<invalidate_num;j++)
		{
			team_string='team';
			team_string=team_string+j;
			var current_value=document.getElementById(team_string).value;
			
			if(parseInt(base_value)== parseInt(current_value))
			{
				exist_flag=true;
				exist_num=j;
				document.getElementById('repeate_groupNumber').value=j;
				break;
			}
		}
		if(exist_flag==true)
		{
			break;
		}
	}
	
	if(exist_flag==true)
	{
		return exist_num;
	}
	
	return 0;
}


function check_freqNumber_repeat()
{
	var freq_string='frePoint';
	var invalidate_num=0;
	var exist_num=0;
	var i=0;
	var j=0;
	var validate_result=0;
	var exist_flag=false;
	
	for(i=1;i<11;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		validate_result=validate_freq_item(freq_string);
		if(0==validate_result)
		{
			invalidate_num=i;
			break;
		}
	}
	if(i==11)
	{
		invalidate_num=11;
	}
	
	for(i=1;i<invalidate_num;i++)
	{
		freq_string='frePoint';
		freq_string=freq_string+i;
		var base_value=document.getElementById(freq_string).value;
		
		for(j=i+1;j<invalidate_num;j++)
		{
			freq_string='frePoint';
			freq_string=freq_string+j;
			var current_value=document.getElementById(freq_string).value;
			
			if(parseFloat(base_value)== parseFloat(current_value))
			{
				exist_flag=true;
				exist_num=j;
				document.getElementById('repeate_freqNumber').value=j;
				break;
			}
		}
		if(exist_flag==true)
		{
			break;
		}
	}
	
	if(exist_flag==true)
	{
		return exist_num;
	}
	
	return 0;
}


function check_blackNumber_repeat()
{
	var blackNumber_string='blkList';
	var invalidate_num=0;
	var exist_num=0;
	var i=0;
	var j=0;
	var validate_result=0;
	var exist_flag=false;
	
	for(i=1;i<11;i++)
	{
		blackNumber_string='blkList';
		blackNumber_string=blackNumber_string+i;
		validate_result=validate_blklist_item(blackNumber_string);
		if(0==validate_result)
		{
			invalidate_num=i;
			break;
		}
	}
	if(i==11)
	{
		invalidate_num=11;
	}
	for(i=1;i<invalidate_num;i++)
	{
		blackNumber_string='blkList';
		blackNumber_string=blackNumber_string+i;
		var base_value=document.getElementById(blackNumber_string).value;
		
		for(j=i+1;j<invalidate_num;j++)
		{
			blackNumber_string='blkList';
			blackNumber_string=blackNumber_string+j;
			var current_value=document.getElementById(blackNumber_string).value;
			
			if(base_value==current_value)
			{
				exist_flag=true;
				exist_num=j;
				document.getElementById('repeate_blackNumber').value=j;
				break;
			}
		}
		if(exist_flag==true)
		{
			break;
		}
	}
	
	if(exist_flag==true)
	{
		return exist_num;
	}
	
	return 0;
}


function check_whiteNumber_repeat()
{
	var whiteNumber_string='whtList';
	var invalidate_num=0;
	var exist_num=0;
	var i=0;
	var j=0;
	var validate_result=0;
	var exist_flag=false;
	
	for(i=1;i<11;i++)
	{
		whiteNumber_string='whtList';
		whiteNumber_string=whiteNumber_string+i;
		validate_result=validate_whtList_item(whiteNumber_string);
		if(0==validate_result)
		{
			invalidate_num=i;
			break;
		}
	}
	if(i==11)
	{
		invalidate_num=11;
	}
	for(i=1;i<invalidate_num;i++)
	{
		whiteNumber_string='whtList';
		whiteNumber_string=whiteNumber_string+i;
		var base_value=document.getElementById(whiteNumber_string).value;
		
		for(j=i+1;j<invalidate_num;j++)
		{
			whiteNumber_string='whtList';
			whiteNumber_string=whiteNumber_string+j;
			var current_value=document.getElementById(whiteNumber_string).value;
			
			if(base_value==current_value)
			{
				exist_flag=true;
				exist_num=j;
				document.getElementById('repeate_whiteNumber').value=j;
				break;
			}
		}
		if(exist_flag==true)
		{
			break;
		}
	}
	
	if(exist_flag==true)
	{
		return exist_num;
	}
	
	return 0;
}


function check_gsmNumber_repeat()
{
	var gsmNumber_string='gsmComm';
	var invalidate_num=0;
	var exist_num=0;
	var i=0;
	var j=0;
	var validate_result=0;
	var exist_flag=false;
	
	for(i=1;i<11;i++)
	{
		gsmNumber_string='gsmComm';
		gsmNumber_string=gsmNumber_string+i;
		validate_result=validate_gsmComm_item(gsmNumber_string);
		if(0==validate_result)
		{
			invalidate_num=i;
			break;
		}
	}
	if(i==11)
	{
		invalidate_num=11;
	}
	for(i=1;i<invalidate_num;i++)
	{
		gsmNumber_string='gsmComm';
		gsmNumber_string=gsmNumber_string+i;
		var base_value=document.getElementById(gsmNumber_string).value;
		
		for(j=i+1;j<invalidate_num;j++)
		{
			gsmNumber_string='gsmComm';
			gsmNumber_string=gsmNumber_string+j;
			var current_value=document.getElementById(gsmNumber_string).value;
			
			if(base_value==current_value)
			{
				exist_flag=true;
				exist_num=j;
				document.getElementById('repeate_gsmNumber').value=j;
				break;
			}
		}
		if(exist_flag==true)
		{
			break;
		}
	}
	
	if(exist_flag==true)
	{
		return exist_num;
	}
	
	return 0;
}
/*
function groupNumber_focus(num)
{
	
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var team_string='team';
			team_string=team_string+j;
			document.getElementById(team_string).disabled=true;
			document.getElementById(team_string).style.color="#000000";
			document.getElementById(team_string).style.borderColor="#aaa";
			document.getElementById(team_string).style.backgroundColor="#FFF";
			document.getElementById(team_string).title="";
		}
		
	}
}

function freqNumber_focus(num)
{
	
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var freq_string='frePoint';
			freq_string=freq_string+j;
			document.getElementById(freq_string).disabled=true;
			document.getElementById(freq_string).style.color="#000000";
			document.getElementById(freq_string).style.borderColor="#aaa";
			document.getElementById(freq_string).style.backgroundColor="#FFF";
			document.getElementById(freq_string).title="";
		}
		
	}
}


function blackNumber_focus(num)
{
	
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var black_string='blkList';
			black_string=black_string+j;
			document.getElementById(black_string).disabled=true;
			document.getElementById(black_string).style.color="#000000";
			document.getElementById(black_string).style.borderColor="#aaa";
			document.getElementById(black_string).style.backgroundColor="#FFF";
			document.getElementById(black_string).title="";
		}
		
	}
}

function whiteNumber_focus(num)
{
	
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var white_string='whtList';
			white_string=white_string+j;
			document.getElementById(white_string).disabled=true;
			document.getElementById(white_string).style.color="#000000";
			document.getElementById(white_string).style.borderColor="#aaa";
			document.getElementById(white_string).style.backgroundColor="#FFF";
			document.getElementById(white_string).title="";
		}
		
	}
}

function gsmNumber_focus(num)
{
	
	for(var j=1;j<11;j++)
	{
		if(j!=num)
		{
			var gsm_string='gsmComm';
			gsm_string=gsm_string+j;
			document.getElementById(gsm_string).disabled=true;
			document.getElementById(gsm_string).style.color="#000000";
			document.getElementById(gsm_string).style.borderColor="#aaa";
			document.getElementById(gsm_string).style.backgroundColor="#FFF";
			document.getElementById(gsm_string).title="";
		}
		
	}
}

function groupNumber_blur(num)
{
	var team_string='team';
	var i=0;
	var j=0;
	team_string=team_string+num;
	var current_value=document.getElementById(team_string).value;
	var validate_result=validate_team_item(team_string);
	
	for(j=1;j<11;j++)
	{
		team_string='team';
		team_string=team_string+j;
		document.getElementById(team_string).disabled=false;
		document.getElementById(team_string).title="组呼呼叫列表；取值范围：1~16776415";
		
	}
	if("" == current_value)
	{
		if(1==num)
		{
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			alarm_tip_groupNumber_return(1,"组1不能为空");
			return ;
		}
		for(i=num+1;i<11;i++)
		{
			team_string='team';
			team_string=team_string+i;
			if(document.getElementById(team_string).value !="")
			{
				break;
			}
		}
		if(i!=11)
		{
			//////////后面存在配置项包含元素/////////////////
			document.getElementById('invalidate_groupNumber').value=num;
			for(j=1;j<11;j++)
			{
				team_string='team';
				team_string=team_string+j;
				document.getElementById(team_string).readOnly=true;
			}
			$("#teamIdNow").empty();
			team_string="组";
			team_string=team_string+num;
			alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
		}
		else
		{
			//////////后面配置项全为空,判断前面/////////////////
			for(i=1;i<num;i++)
			{
				team_string='team';
				team_string=team_string+i;
				if (0==validate_team_item(team_string))
				{
					for( j=i+1;j<num;j++)
					{
						team_string='team';
						team_string=team_string+j;
						if(document.getElementById(team_string).value !="")
						{
							break;
						}
					}
					if(j!=num)
					{
						document.getElementById('invalidate_groupNumber').value=i;
						for(j=1;j<11;j++)
						{
							team_string='team';
							team_string=team_string+j;
							document.getElementById(team_string).readOnly=true;
						}
						$("#teamIdNow").empty();
						team_string="组";
						team_string=team_string+i;
						alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
					}
					else
					{
						team_string='team';
						team_string=team_string+i;
						if(""==document.getElementById(team_string).value)
						{
							add_id_option();
						}
						else
						{
							document.getElementById('invalidate_groupNumber').value=i;
							for(j=1;j<11;j++)
							{
								team_string='team';
								team_string=team_string+j;
								document.getElementById(team_string).readOnly=true;
							}
							$("#teamIdNow").empty();
							team_string="组";
							team_string=team_string+i;
							alarm_tip_groupNumber_return(1,team_string+"输入格式错误");
						}
					}
					break;
				}
			}
			if(i==num)
			{
				add_id_option();
			}
		}
	}
	else if(0 == validate_result)
	{
		////不为空，且无效////
		for(i=1;i<num;i++)
		{
			team_string='team';
			team_string=team_string+i;
			if (0==validate_team_item(team_string))
			{
				document.getElementById('invalidate_groupNumber').value=i;
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=true;
				}
				$("#teamIdNow").empty();
				team_string="组";
				team_string=team_string+i;
				alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///前面均有效，判断后面////
			for(i=num+1;i<11;i++)
			{
				team_string='team';
				team_string=team_string+i;
				if(""!=document.getElementById(team_string).value)
				{
					document.getElementById('invalidate_groupNumber').value=num;
					for(j=1;j<11;j++)
					{
						team_string='team';
						team_string=team_string+j;
						document.getElementById(team_string).readOnly=true;
					}
					$("#teamIdNow").empty();
					team_string="组";
					team_string=team_string+num;
					alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
					break;
				}
			}
			if(i==11)
			{
				//////前后均有效/////
				document.getElementById('invalidate_groupNumber').value=num;
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=true;
				}
				$("#teamIdNow").empty();
				team_string="组";
				team_string=team_string+num;
				alarm_tip_groupNumber_return(1,team_string+"输入格式错误");
			}
		}
	}
	else
	{
		for(i=1;i<num;i++)
		{
			team_string='team';
			team_string=team_string+i;
			if (0==validate_team_item(team_string))
			{
				document.getElementById('invalidate_groupNumber').value=i;
				for(j=1;j<11;j++)
				{
					team_string='team';
					team_string=team_string+j;
					document.getElementById(team_string).readOnly=true;
				}
				$("#teamIdNow").empty();
				team_string="组";
				team_string=team_string+i;
				alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///////	前面数据都有效////
			for(i=1;i<num;i++)
			{
				team_string='team';
				team_string=team_string+i;
				if(parseInt(document.getElementById(team_string).value)==parseInt(current_value))
				{
					/////重复/////////
					document.getElementById('invalidate_groupNumber').value=num;
					for(j=1;j<11;j++)
					{
						team_string='team';
						team_string=team_string+j;
						document.getElementById(team_string).readOnly=true;
					}
					$("#teamIdNow").empty();
					team_string="组";
					team_string=team_string+num;
					alarm_tip_groupNumber_return(1,team_string+"输入值已存在");
					break;
				}
			}
			if(i==num)
			{
				/////说明前面均有效，判断后面////
				for(i=num+1;i<11;i++)
				{
					team_string='team';
					team_string=team_string+i;
					if (0==validate_team_item(team_string))
					{
						for( j=i+1;j<11;j++)
						{
							team_string='team';
							team_string=team_string+j;
							if(document.getElementById(team_string).value !="")
							{
								break;
							}
						}
						if(j!=11)
						{
							document.getElementById('invalidate_groupNumber').value=i;
							for(j=1;j<11;j++)
							{
								team_string='team';
								team_string=team_string+j;
								document.getElementById(team_string).readOnly=true;
							}
							$("#teamIdNow").empty();
							team_string="组";
							team_string=team_string+i;
							alarm_tip_groupNumber(2,team_string+'输入格式错误，为保证组呼列表连续是否清空后续列表');
						}
						else
						{
							team_string='team';
							team_string=team_string+i;
							if(document.getElementById(team_string).value=="")
							{
								add_id_option();
							}
							else
							{
								document.getElementById('invalidate_groupNumber').value=i;
								for(j=1;j<11;j++)
								{
									team_string='team';
									team_string=team_string+j;
									document.getElementById(team_string).readOnly=true;
								}
								$("#teamIdNow").empty();
								team_string="组";
								team_string=team_string+i;
								alarm_tip_groupNumber_return(1,team_string+"输入格式错误");
							}
						}
					}
					break;
				}
				if(i==11)
				{
					add_id_option();
				}
				
			}
			
		}
	}
}

function freqNumber_blur(num)
{
	var freq_string='frePoint';
	var i=0;
	var j=0;
	freq_string=freq_string+num;
	var current_value=document.getElementById(freq_string).value;
	var validate_result=validate_freq_item(freq_string);
	
	for(j=1;j<11;j++)
	{
		freq_string='frePoint';
		freq_string=freq_string+j;
		document.getElementById(freq_string).disabled=false;
		document.getElementById(freq_string).title="设备可用频率列表；取值范围：350~400；输入格式：最多6位精度位数";
		
	}
	if("" == current_value)
	{
		if(1==num)
		{
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			alarm_tip_freqNumber_return(1,"频率1不能为空");
			return ;
		}
		for(i=num+1;i<11;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;
			if(document.getElementById(freq_string).value !="")
			{
				break;
			}
		}
		if(i!=11)
		{
			//////////后面存在配置项包含元素/////////////////
			document.getElementById('invalidate_freqNumber').value=num;
			for(j=1;j<11;j++)
			{
				freq_string='frePoint';
				freq_string=freq_string+j;
				document.getElementById(freq_string).readOnly=true;
			}
			$("#frePointNow").empty();
			freq_string="频率";
			freq_string=freq_string+num;
			alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
		}
		else
		{
			//////////后面配置项全为空,判断前面/////////////////
			for(i=1;i<num;i++)
			{
				freq_string='frePoint';
				freq_string=freq_string+i;
				if (0==validate_freq_item(freq_string))
				{
					for( j=i+1;j<num;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						if(document.getElementById(freq_string).value !="")
						{
							break;
						}
					}
					if(j!=num)
					{
						document.getElementById('invalidate_freqNumber').value=i;
						for(j=1;j<11;j++)
						{
							freq_string='frePoint';
							freq_string=freq_string+j;
							document.getElementById(freq_string).readOnly=true;
						}
						$("#frePointNow").empty();
						freq_string="频率";
						freq_string=freq_string+i;
						alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
					}
					else
					{
						freq_string='frePoint';
						freq_string=freq_string+i;
						if(""==document.getElementById(freq_string).value)
						{
							add_freq_option();
						}
						else
						{
							document.getElementById('invalidate_freqNumber').value=i;
							for(j=1;j<11;j++)
							{
								freq_string='frePoint';
								freq_string=freq_string+j;
								document.getElementById(freq_string).readOnly=true;
							}
							$("#frePointNow").empty();
							freq_string="频率";
							freq_string=freq_string+i;
							alarm_tip_freqNumber_return(1,freq_string+"输入格式错误");
						}
					}
					break;
				}
			}
			if(i==num)
			{
				/////说明前面均有效，判断后面////
				add_freq_option();
				
			}
		}
	}
	else if(0 == validate_result)
	{
		////不为空，且无效////
		for(i=1;i<num;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;
			if (0==validate_freq_item(freq_string))
			{
				document.getElementById('invalidate_freqNumber').value=i;
				for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=true;
				}
				$("#frePointNow").empty();
				freq_string="频率";
				freq_string=freq_string+i;
				alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///前面均有效，判断后面////
			for(i=num+1;i<11;i++)
			{
				freq_string='frePoint';
				freq_string=freq_string+i;
				if(""!=document.getElementById(freq_string).value)
				{
					document.getElementById('invalidate_freqNumber').value=num;
					for(j=1;j<11;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						document.getElementById(freq_string).readOnly=true;
					}
					$("#frePointNow").empty();
					freq_string="频率";
					freq_string=freq_string+num;
					alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
					break;
				}
			}
			if(i==11)
			{
				//////前后均有效/////
				document.getElementById('invalidate_freqNumber').value=num;
				for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=true;
				}
				$("#frePointNow").empty();
				freq_string="频率";
				freq_string=freq_string+num;
				alarm_tip_freqNumber_return(1,freq_string+"输入格式错误");
			}
		}
	}
	else
	{
		for(i=1;i<num;i++)
		{
			freq_string='frePoint';
			freq_string=freq_string+i;
			if (0==validate_freq_item(freq_string))
			{
				document.getElementById('invalidate_freqNumber').value=i;
				for(j=1;j<11;j++)
				{
					freq_string='frePoint';
					freq_string=freq_string+j;
					document.getElementById(freq_string).readOnly=true;
				}
				$("#frePointNow").empty();
				freq_string="频率";
				freq_string=freq_string+i;
				alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///////	前面数据都有效////
			for(i=1;i<num;i++)
			{
				freq_string='frePoint';
				freq_string=freq_string+i;
				if(parseFloat(document.getElementById(freq_string).value)==parseFloat(current_value))
				{
					/////重复/////////
					document.getElementById('invalidate_freqNumber').value=num;
					for(j=1;j<11;j++)
					{
						freq_string='frePoint';
						freq_string=freq_string+j;
						document.getElementById(freq_string).readOnly=true;
					}
					$("#frePointNow").empty();
					freq_string="频率";
					freq_string=freq_string+num;
					alarm_tip_freqNumber_return(1,freq_string+"输入值已存在");
					break;
				}
			}
			if(i==num)
			{
				for(i=num+1;i<11;i++)
				{
					freq_string='frePoint';
					freq_string=freq_string+i;
					if (0==validate_freq_item(freq_string))
					{
						for( j=i+1;j<11;j++)
						{
							freq_string='frePoint';
							freq_string=freq_string+j;
							if(document.getElementById(freq_string).value !="")
							{
								break;
							}
						}
						if(j!=11)
						{
							document.getElementById('invalidate_freqNumber').value=i;
							for(j=1;j<11;j++)
							{
								freq_string='frePoint';
								freq_string=freq_string+j;
								document.getElementById(freq_string).readOnly=true;
							}
							$("#frePointNow").empty();
							freq_string="频率";
							freq_string=freq_string+i;
							alarm_tip_freqNumber(2,freq_string+'输入格式错误，为保证频率列表连续是否清空后续列表');
						}
						else
						{
							freq_string='frePoint';
							freq_string=freq_string+i;
							if(document.getElementById(freq_string).value=="")
							{
								add_freq_option();
							}
							else
							{
								document.getElementById('invalidate_freqNumber').value=i;
								for(j=1;j<11;j++)
								{
									freq_string='frePoint';
									freq_string=freq_string+j;
									document.getElementById(freq_string).readOnly=true;
								}
								$("#frePointNow").empty();
								freq_string="频率";
								freq_string=freq_string+i;
								alarm_tip_freqNumber_return(1,freq_string+"输入格式错误");
							}
						}
					}
					break;
				}
				if(i==11)
				{
					add_freq_option();
				}
			}
			
		}
	}
}




function blackNumber_blur(num)
{
	var black_string='blkList';
	var i=0;
	var j=0;
	black_string=black_string+num;
	var current_value=document.getElementById(black_string).value;
	var validate_result=validate_blklist_item(black_string);
	
	for(j=1;j<11;j++)
	{
		black_string='blkList';
		black_string=black_string+j;
		document.getElementById(black_string).disabled=false;
		document.getElementById(black_string).title="黑名单列表，列入该名单的电话号码将被屏蔽呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		
	}
	if("" == current_value)
	{
		for(i=num+1;i<11;i++)
		{
			black_string='blkList';
			black_string=black_string+i;
			if(document.getElementById(black_string).value !="")
			{
				break;
			}
		}
		if(i!=11)
		{
			//////////后面存在配置项包含元素/////////////////
			document.getElementById('invalidate_blackNumber').value=num;
			for(j=1;j<11;j++)
			{
				black_string='blkList';
				black_string=black_string+j;
				document.getElementById(black_string).readOnly=true;
			}
			black_string="黑名单";
			black_string=black_string+num;
			alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
		}
		else
		{
			//////////后面配置项全为空,判断前面/////////////////
			for(i=1;i<num;i++)
			{
				black_string='blkList';
				black_string=black_string+i;
				if (0==validate_blklist_item(black_string))
				{
					for( j=i+1;j<num;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						if(document.getElementById(black_string).value !="")
						{
							break;
						}
					}
					if(j!=num)
					{
						document.getElementById('invalidate_blackNumber').value=i;
						for(j=1;j<11;j++)
						{
							black_string='blkList';
							black_string=black_string+j;
							document.getElementById(black_string).readOnly=true;
						}
						black_string="黑名单";
						black_string=black_string+i;
						alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
					}
					else
					{
						black_string='blkList';
						black_string=black_string+i;
						if(""!=document.getElementById(black_string).value)
						{
							document.getElementById('invalidate_blackNumber').value=i;
							for(j=1;j<11;j++)
							{
								black_string='blkList';
								black_string=black_string+j;
								document.getElementById(black_string).readOnly=true;
							}
							black_string="黑名单";
							black_string=black_string+i;
							alarm_tip_blackNumber_return(1,black_string+"输入格式错误");
						}
					}
					break;
				}
			}
		}
	}
	else if(0 == validate_result)
	{
		////不为空，且无效////
		for(i=1;i<num;i++)
		{
			black_string='blkList';
			black_string=black_string+i;
			if (0==validate_blklist_item(black_string))
			{
				document.getElementById('invalidate_blackNumber').value=i;
				for(j=1;j<11;j++)
				{
					black_string='blkList';
					black_string=black_string+j;
					document.getElementById(black_string).readOnly=true;
				}
				black_string="黑名单";
				black_string=black_string+i;
				alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///前面均有效，判断后面////
			for(i=num+1;i<11;i++)
			{
				black_string='blkList';
				black_string=black_string+i;
				if(""!=document.getElementById(black_string).value)
				{
					document.getElementById('invalidate_blackNumber').value=num;
					for(j=1;j<11;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						document.getElementById(black_string).readOnly=true;
					}
					black_string='黑名单';
					black_string=black_string+num;
					alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
					break;
				}
			}
			if(i==11)
			{
				//////前后均有效/////
				document.getElementById('invalidate_blackNumber').value=num;
				for(j=1;j<11;j++)
				{
					black_string='blkList';
					black_string=black_string+j;
					document.getElementById(black_string).readOnly=true;
				}
				black_string="黑名单";
				black_string=black_string+num;
				alarm_tip_blackNumber_return(1,black_string+"输入格式错误");
			}
		}
	}
	else
	{
		for(i=1;i<num;i++)
		{
			black_string='blkList';
			black_string=black_string+i;
			if (0==validate_blklist_item(black_string))
			{
				document.getElementById('invalidate_blackNumber').value=i;
				for(j=1;j<11;j++)
				{
					black_string='blkList';
					black_string=black_string+j;
					document.getElementById(black_string).readOnly=true;
				}
				black_string="黑名单";
				black_string=black_string+i;
				alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///////	前面数据都有效////
			for(i=1;i<num;i++)
			{
				black_string='blkList';
				black_string=black_string+i;
				if(parseInt(document.getElementById(black_string).value)==parseInt(current_value))
				{
					/////重复/////////
					document.getElementById('invalidate_blackNumber').value=num;
					for(j=1;j<11;j++)
					{
						black_string='blkList';
						black_string=black_string+j;
						document.getElementById(black_string).readOnly=true;
					}
					black_string="黑名单";
					black_string=black_string+num;
					alarm_tip_blackNumber_return(1,black_string+"输入值已存在");
					break;
				}
			}
			if(i==num)
			{
				/////说明前面均有效，判断后面////
				for(i=num+1;i<11;i++)
				{
					black_string='blkList';
					black_string=black_string+i;
					if (0==validate_blklist_item(black_string))
					{
						for( j=i+1;j<11;j++)
						{
							black_string='blkList';
							black_string=black_string+j;
							if(document.getElementById(black_string).value !="")
							{
								break;
							}
						}
						if(j!=11)
						{
							document.getElementById('invalidate_blackNumber').value=i;
							for(j=1;j<11;j++)
							{
								black_string='blkList';
								black_string=black_string+j;
								document.getElementById(black_string).readOnly=true;
							}
							black_string="黑名单";
							black_string=black_string+i;
							alarm_tip_blackNumber(2,black_string+'输入格式错误，为保证黑名单列表连续是否清空后续列表');
						}
						else
						{
							black_string='blkList';
							black_string=black_string+i;
							if(document.getElementById(black_string).valu!="")
							{
								document.getElementById('invalidate_blackNumber').value=i;
								for(j=1;j<11;j++)
								{
									black_string='blkList';
									black_string=black_string+j;
									document.getElementById(black_string).readOnly=true;
								}
								black_string="黑名单";
								black_string=black_string+i;
								alarm_tip_blackNumber_return(1,black_string+"输入格式错误");
							}
						}
					}
					break;
				}
				
			}
			
		}
	}
}


function whiteNumber_blur(num)
{
	var white_string='whtList';
	var i=0;
	var j=0;
	white_string=white_string+num;
	var current_value=document.getElementById(white_string).value;
	var validate_result=validate_blklist_item(white_string);
	
	for(j=1;j<11;j++)
	{
		white_string='whtList';
		white_string=white_string+j;
		document.getElementById(white_string).disabled=false;
		document.getElementById(white_string).title="白名单列表，列入该名单的电话号码将被开启呼叫功能；输入格式：最多为30字符，且字符只能为数字";
		
	}
	if("" == current_value)
	{
		for(i=num+1;i<11;i++)
		{
			white_string='whtList';
			white_string=white_string+i;
			if(document.getElementById(white_string).value !="")
			{
				break;
			}
		}
		if(i!=11)
		{
			//////////后面存在配置项包含元素/////////////////
			document.getElementById('invalidate_whiteNumber').value=num;
			for(j=1;j<11;j++)
			{
				white_string='whtList';
				white_string=white_string+j;
				document.getElementById(white_string).readOnly=true;
			}
			white_string="白名单";
			white_string=white_string+num;
			alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
		}
		else
		{
			//////////后面配置项全为空,判断前面/////////////////
			for(i=1;i<num;i++)
			{
				white_string='whtList';
				white_string=white_string+i;
				if (0==validate_whtList_item(white_string))
				{
					for( j=i+1;j<num;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						if(document.getElementById(white_string).value !="")
						{
							break;
						}
					}
					if(j!=num)
					{
						document.getElementById('invalidate_whiteNumber').value=i;
						for(j=1;j<11;j++)
						{
							white_string='whtList';
							white_string=white_string+j;
							document.getElementById(white_string).readOnly=true;
						}
						white_string="白名单";
						white_string=white_string+i;
						alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
					}
					else
					{
						white_string='whtList';
						white_string=white_string+i;
						if(""!=document.getElementById(white_string).value)
						{
							document.getElementById('invalidate_whiteNumber').value=i;
							for(j=1;j<11;j++)
							{
								white_string='whtList';
								white_string=white_string+j;
								document.getElementById(white_string).readOnly=true;
							}
							white_string="白名单";
							white_string=white_string+i;
							alarm_tip_whiteNumber_return(1,white_string+"输入格式错误");
						}
					}
					break;
				}
			}
		}
	}
	else if(0 == validate_result)
	{
		////不为空，且无效////
		for(i=1;i<num;i++)
		{
			white_string='whtList';
			white_string=white_string+i;
			if (0==validate_whtList_item(white_string))
			{
				document.getElementById('invalidate_whiteNumber').value=i;
				for(j=1;j<11;j++)
				{
					white_string='whtList';
					white_string=white_string+j;
					document.getElementById(white_string).readOnly=true;
				}
				white_string="白名单";
				white_string=white_string+i;
				alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///前面均有效，判断后面////
			for(i=num+1;i<11;i++)
			{
				white_string='whtList';
				white_string=white_string+i;
				if(""!=document.getElementById(white_string).value)
				{
					document.getElementById('invalidate_whiteNumber').value=num;
					for(j=1;j<11;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						document.getElementById(white_string).readOnly=true;
					}
					white_string='白名单';
					white_string=white_string+num;
					alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
					break;
				}
			}
			if(i==11)
			{
				//////前后均有效/////
				document.getElementById('invalidate_whiteNumber').value=num;
				for(j=1;j<11;j++)
				{
					white_string='whtList';
					white_string=white_string+j;
					document.getElementById(white_string).readOnly=true;
				}
				white_string="白名单";
				white_string=white_string+num;
				alarm_tip_whiteNumber_return(1,white_string+"输入格式错误");
			}
		}
	}
	else
	{
		for(i=1;i<num;i++)
		{
			white_string='whtList';
			white_string=white_string+i;
			if (0==validate_whtList_item(white_string))
			{
				document.getElementById('invalidate_whiteNumber').value=i;
				for(j=1;j<11;j++)
				{
					white_string='whtList';
					white_string=white_string+j;
					document.getElementById(white_string).readOnly=true;
				}
				white_string="白名单";
				white_string=white_string+i;
				alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///////	前面数据都有效////
			for(i=1;i<num;i++)
			{
				white_string='whtList';
				white_string=white_string+i;
				if(parseInt(document.getElementById(white_string).value)==parseInt(current_value))
				{
					/////重复/////////
					document.getElementById('invalidate_whiteNumber').value=num;
					for(j=1;j<11;j++)
					{
						white_string='whtList';
						white_string=white_string+j;
						document.getElementById(white_string).readOnly=true;
					}
					white_string="白名单";
					white_string=white_string+num;
					alarm_tip_whiteNumber_return(1,white_string+"输入值已存在");
					break;
				}
			}
			if(i==num)
			{
				/////说明前面均有效，判断后面////
				for(i=num+1;i<11;i++)
				{
					white_string='whtList';
					white_string=white_string+i;
					if (0==validate_whtList_item(white_string))
					{
						for( j=i+1;j<11;j++)
						{
							white_string='whtList';
							white_string=white_string+j;
							if(document.getElementById(white_string).value !="")
							{
								break;
							}
						}
						if(j!=11)
						{
							document.getElementById('invalidate_whiteNumber').value=i;
							for(j=1;j<11;j++)
							{
								white_string='whtList';
								white_string=white_string+j;
								document.getElementById(white_string).readOnly=true;
							}
							white_string="白名单";
							white_string=white_string+i;
							alarm_tip_whiteNumber(2,white_string+'输入格式错误，为保证白名单列表连续是否清空后续列表');
						}
						else
						{
							white_string='whtList';
							white_string=white_string+i;
							if(document.getElementById(white_string).valu!="")
							{
								document.getElementById('invalidate_whiteNumber').value=i;
								for(j=1;j<11;j++)
								{
									white_string='whtList';
									white_string=white_string+j;
									document.getElementById(white_string).readOnly=true;
								}
								white_string="白名单";
								white_string=white_string+i;
								alarm_tip_whiteNumber_return(1,white_string+"输入格式错误");
							}
						}
					}
					break;
				}
				
			}
			
		}
	}
}


function gsmNumber_blur(num)
{
	var gsm_string='gsmComm';
	var i=0;
	var j=0;
	gsm_string=gsm_string+num;
	var current_value=document.getElementById(gsm_string).value;
	var validate_result=validate_blklist_item(gsm_string);
	
	for(j=1;j<11;j++)
	{
		gsm_string='gsmComm';
		gsm_string=gsm_string+j;
		document.getElementById(gsm_string).disabled=false;
		document.getElementById(gsm_string).title="GSM模块呼叫号码列表；输入格式：最多为30字符，且字符只能为数字";
		
	}
	if("" == current_value)
	{
		for(i=num+1;i<11;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;
			if(document.getElementById(gsm_string).value !="")
			{
				break;
			}
		}
		if(i!=11)
		{
			//////////后面存在配置项包含元素/////////////////
			document.getElementById('invalidate_gsmNumber').value=num;
			for(j=1;j<11;j++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+j;
				document.getElementById(gsm_string).readOnly=true;
			}
			gsm_string="联系人";
			gsm_string=gsm_string+num;
			alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
		}
		else
		{
			//////////后面配置项全为空,判断前面/////////////////
			for(i=1;i<num;i++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+i;
				if (0==validate_gsmComm_item(gsm_string))
				{
					for( j=i+1;j<num;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						if(document.getElementById(gsm_string).value !="")
						{
							break;
						}
					}
					if(j!=num)
					{
						document.getElementById('invalidate_gsmNumber').value=i;
						for(j=1;j<11;j++)
						{
							gsm_string='gsmComm';
							gsm_string=gsm_string+j;
							document.getElementById(gsm_string).readOnly=true;
						}
						gsm_string="联系人";
						gsm_string=gsm_string+i;
						alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
					}
					else
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+i;
						if(""!=document.getElementById(gsm_string).value)
						{
							document.getElementById('invalidate_gsmNumber').value=i;
							for(j=1;j<11;j++)
							{
								gsm_string='gsmComm';
								gsm_string=gsm_string+j;
								document.getElementById(gsm_string).readOnly=true;
							}
							gsm_string="联系人";
							gsm_string=gsm_string+i;
							alarm_tip_gsmNumber_return(1,gsm_string+"输入格式错误");
						}
					}
					break;
				}
			}
		}
	}
	else if(0 == validate_result)
	{
		////不为空，且无效////
		for(i=1;i<num;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;
			if (0==validate_gsmComm_item(gsm_string))
			{
				document.getElementById('invalidate_gsmNumber').value=i;
				for(j=1;j<11;j++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+j;
					document.getElementById(gsm_string).readOnly=true;
				}
				gsm_string="联系人";
				gsm_string=gsm_string+i;
				alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///前面均有效，判断后面////
			for(i=num+1;i<11;i++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+i;
				if(""!=document.getElementById(gsm_string).value)
				{
					document.getElementById('invalidate_gsmNumber').value=num;
					for(j=1;j<11;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						document.getElementById(gsm_string).readOnly=true;
					}
					gsm_string='联系人';
					gsm_string=gsm_string+num;
					alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
					break;
				}
			}
			if(i==11)
			{
				//////前后均有效/////
				document.getElementById('invalidate_gsmNumber').value=num;
				for(j=1;j<11;j++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+j;
					document.getElementById(gsm_string).readOnly=true;
				}
				gsm_string='联系人';
				gsm_string=gsm_string+num;
				alarm_tip_gsmNumber_return(1,gsm_string+"输入格式错误");
			}
		}
	}
	else
	{
		for(i=1;i<num;i++)
		{
			gsm_string='gsmComm';
			gsm_string=gsm_string+i;
			if (0==validate_gsmComm_item(gsm_string))
			{
				document.getElementById('invalidate_gsmNumber').value=i;
				for(j=1;j<11;j++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+j;
					document.getElementById(gsm_string).readOnly=true;
				}
				gsm_string='联系人';
				gsm_string=gsm_string+i;
				alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
				break;
			}
		}
		if(i==num)
		{
			///////	前面数据都有效////
			for(i=1;i<num;i++)
			{
				gsm_string='gsmComm';
				gsm_string=gsm_string+i;
				if(parseInt(document.getElementById(gsm_string).value)==parseInt(current_value))
				{
					/////重复/////////
					document.getElementById('invalidate_gsmNumber').value=num;
					for(j=1;j<11;j++)
					{
						gsm_string='gsmComm';
						gsm_string=gsm_string+j;
						document.getElementById(gsm_string).readOnly=true;
					}
					gsm_string='联系人';
					gsm_string=gsm_string+num;
					alarm_tip_gsmNumber_return(1,gsm_string+"输入值已存在");
					break;
				}
			}
			if(i==num)
			{
				/////说明前面均有效，判断后面////
				for(i=num+1;i<11;i++)
				{
					gsm_string='gsmComm';
					gsm_string=gsm_string+i;
					if (0==validate_gsmComm_item(gsm_string))
					{
						for( j=i+1;j<11;j++)
						{
							gsm_string='gsmComm';
							gsm_string=gsm_string+j;
							if(document.getElementById(gsm_string).value !="")
							{
								break;
							}
						}
						if(j!=11)
						{
							document.getElementById('invalidate_gsmNumber').value=i;
							for(j=1;j<11;j++)
							{
								gsm_string='gsmComm';
								gsm_string=gsm_string+j;
								document.getElementById(gsm_string).readOnly=true;
							}
							gsm_string='联系人';
							gsm_string=gsm_string+i;
							alarm_tip_gsmNumber(2,gsm_string+'输入格式错误，为保证联系人列表连续是否清空后续列表');
						}
						else
						{
							gsm_string='gsmComm';
							gsm_string=gsm_string+i;
							if(document.getElementById(gsm_string).valu!="")
							{
								document.getElementById('invalidate_gsmNumber').value=i;
								for(j=1;j<11;j++)
								{
									gsm_string='gsmComm';
									gsm_string=gsm_string+j;
									document.getElementById(gsm_string).readOnly=true;
								}
								gsm_string='联系人';
								gsm_string=gsm_string=+i;
								alarm_tip_gsmNumber_return(1,gsm_string=+"输入格式错误");
							}
						}
					}
					break;
				}
				
			}
			
		}
	}
}

*/
function validate_team()
{
	for(var i=1;i<11;i++)
    {
	    var team_string='team';
		team_string=team_string+i;
		if(i==1)
		{
			if(validate_team_item(team_string)==0)
			{
				var index=i;
				//alarm_tip(team_num+'输入格式错误');
				i++;
				for(;i<11;i++)
				{   
					team_string='team';
					team_string=team_string+i;
					document.getElementById(team_string).value="";
				//	document.getElementById(team_string).disabled=true;
				}
				return index;
			}
		}
		else
		{
			if(document.getElementById(team_string).value != "")
			{
				if(validate_team_item(team_string)==0)
				{
					var index=i;
					//	alarm_tip(team_num+'输入格式错误');
					/*
					i++;
				    for(;i<11;i++)
					{   
						team_string='team';
						team_string=team_string+i;
						document.getElementById(team_string).value="";
						//	document.getElementById(team_string).disabled=true;
					}
					*/
					return index;
				}
			}
			else
			{
				/*
				i++;
				for(;i<11;i++)
				{   
					team_string='team';
					team_string=team_string+i;
					document.getElementById(team_string).value="";
				//	document.getElementById(team_string).disabled=true;
				}
				*/
				return 0;
			}
		}
		
    }
	return 0;
}

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

function validate_freq()
{
	
	for(var i=1;i<11;i++)
    {
	    var freq_string='frePoint';
		freq_string=freq_string+i;
		
		
		if(document.getElementById(freq_string).value != "")
		{
			if(validate_freq_item(freq_string)==0)
			{
				var index=i;
				//	alarm_tip(freq_num+'输入格式错误');
				/*
				i++;
				for(;i<11;i++)
				{   
					freq_string='frePoint';
					freq_string=freq_string+i;
					document.getElementById(freq_string).value="";
					//document.getElementById(freq_string).disabled=true;
				}
				*/
				return index;
			}
		}
		else
		{
			/*
			i++;
			for(;i<11;i++)
			{   
				freq_string='frePoint';
				freq_string=freq_string+i;
				document.getElementById(freq_string).value="";
			//	document.getElementById(freq_string).disabled=true;
			}
			*/
			return 0;
		}
	}
	return 0;
}
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


function validate_blklist()
{
	
	for(var i=1;i<11;i++)
    {
	    var blklist_string='blkList';
		blklist_string=blklist_string+i;
		
		if(document.getElementById(blklist_string).value != "")
		{
			if(validate_blklist_item(blklist_string)==0)
			{
				var index=i;
				//	alarm_tip(freq_num+'输入格式错误');
				/*
				i++;
				for(;i<11;i++)
				{   
					blklist_string='blkList';
					blklist_string=blklist_string+i;
					document.getElementById(blklist_string).value="";
				//	document.getElementById(blklist_string).disabled=true;
				}
				*/
				return index;
			}
		}
		else
		{
			/*
			i++;
			for(;i<11;i++)
			{   
				blklist_string='blkList';
				blklist_string=blklist_string+i;
				document.getElementById(blklist_string).value="";
		//		document.getElementById(blklist_string).disabled=true;
			}
			*/
			return 0;
		}
	}
	return 0;
}

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


function validate_whtList()
{
	
	for(var i=1;i<11;i++)
    {
	    var whtList_string='whtList';
		whtList_string=whtList_string+i;
		
		if(document.getElementById(whtList_string).value != "")
		{
			if(validate_blklist_item(whtList_string)==0)
			{
				var index=i;
				/*
				//	alarm_tip(freq_num+'输入格式错误');
				i++;
				for(;i<11;i++)
				{   
					whtList_string='whtList';
					whtList_string=whtList_string+i;
					document.getElementById(whtList_string).value="";
				//	document.getElementById(whtList_string).disabled=true;
				}
				*/
				return index;
			}
		}
		else
		{
			/*
			i++;
			for(;i<11;i++)
			{   
				whtList_string='whtList';
				whtList_string=whtList_string+i;
				document.getElementById(whtList_string).value="";
			//	document.getElementById(whtList_string).disabled=true;
			}
			*/
			return 0;
		}
	}
	return 0;
}

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


function validate_gsmComm()
{
	
	for(var i=1;i<11;i++)
    {
	    var gsmComm_string='gsmComm';
		gsmComm_string=gsmComm_string+i;
		
		if(document.getElementById(gsmComm_string).value != "")
		{
			if(validate_gsmComm_item(gsmComm_string)==0)
			{
				var index=i;
				/*
				//	alarm_tip(freq_num+'输入格式错误');
				i++;
				for(;i<11;i++)
				{   
					gsmComm_string='gsmComm';
					gsmComm_string=gsmComm_string+i;
					document.getElementById(gsmComm_string).value="";
				//	document.getElementById(gsmComm_string).disabled=true;
				}
				*/
				return index;
			}
		}
		else
		{
			/*
			i++;
			for(;i<11;i++)
			{   
				gsmComm_string='gsmComm';
				gsmComm_string=gsmComm_string+i;
				document.getElementById(gsmComm_string).value="";
			//	document.getElementById(gsmComm_string).disabled=true;
			}*/
			return 0;
		}
	}
	return 0;
}

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
///////验证下拉列表///////////

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
		document.getElementById('intercomID').value ="";
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
	if(team_return!=0)
	{	
		var team_num='team'+team_return;
		document.getElementById(team_num).value ="";
	}
	/*
	if(validate_frePointNow()==0)
	{
		document.getElementById('frePointNow').value =350.0000;
    }
	*/
	var freq_return=validate_freq();
	if(freq_return!=0)
	{
		var freq_num='frePoint'+freq_return;
		document.getElementById(freq_num).value ="";
	}
	
	var gsmComm_return=validate_gsmComm();
	if(gsmComm_return!=0)
	{
		var gsmComm_num='gsmComm'+gsmComm_return;
		document.getElementById(whtList_num).value ="";
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
		if(blklist_return!=0)
		{
			var blklist_num='blkList'+blklist_return;
			document.getElementById(blklist_num).value ="";
		}
		var whtList_return=validate_whtList();
		if(whtList_return!=0)
		{
			var whtList_num='whtList'+whtList_return;
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
		var ret=/^.{5,20}$/;
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

function add_id_option()
{
	var index=11;
	for(var i=1;i<11;i++)
	{
		var team_string='team';
		team_string=team_string+i;
		
		if(validate_team_item(team_string)==0)
		{
			index=i;
		//		document.getElementById(team_string).value="";
		//		document.getElementById(team_string).style.border-color="#aaa";
		/*
				i++;
				for(;i<11;i++)
				{   
					team_string='team';
					team_string=team_string+i;
					document.getElementById(team_string).value="";
		//			document.getElementById(team_string).style.border-color="#aaa";
					document.getElementById(team_string).disabled=true;
				}*/
			break;
		}
	}
	
	
	if(index ==1)
	{
		$("#teamIdNow").empty();
	//	$("#teamIdNow").append( "<option value=\"0\">"+" "+"</option>" );
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
				$("#teamIdNow").append( "<option value="+team_value+">"+"组"+i+"--"+team_value+"</option>" );	
			}
			
		}
	}
}



function add_freq_option()
{
	var index=11;
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
				$("#frePointNow").append( "<option value="+tmp+">"+"频率"+i+"--"+team_value+"</option>" );	
			}
			
		}
	}
}


function alarm_tip_user(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("old_user").style.borderColor="red";
																			document.getElementById("old_user").focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}

function alarm_tip_code(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("old_code").style.borderColor="red";
																			document.getElementById("old_code").focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}

function alarm_tip_confirm_code(btns_num,option_info){
	$.layer({
				area:['400px','150px'],
				dialog:{btns : btns_num,type:1,msg:option_info,
													yes : function(index){ 
																			layer.close(index);
																			document.getElementById("confirm_code").style.borderColor="red";
																			document.getElementById("confirm_code").focus();
																			
																			}},
				title : ['温馨提示',true],
				offset: ['300px', '50%']		
				});
}

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
	
	///验证原用户名/////
	if(user != web_user)
	{
		alarm_tip_user(1,'该用户名不存在');
		return;
	}
	else
	{
		document.getElementById("old_user").style.borderColor="#aaa";
	}
	
	if(ret.test(new_user)== 0)
	{
		alarm_tip(1,'新用户名输入无效');
		return;	
	}
	if(old_code != web_code)
	{
		alarm_tip_code(1,'密码错误');
		return;
	}
	else
	{
		document.getElementById("old_code").style.borderColor="#aaa";
	}
	if(ret.test(new_code)== 0)
	{
		alarm_tip(1,'新密码输入无效');
		return;	
	}
	if(new_code != confirm_code)
	{
		alarm_tip_confirm_code(1,'两次密码输入不一致');
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
		layer.load('保存用户信息中，请等待。。。。');
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
	layer.load('保存用户信息中，请等待。。。。');
}


/**
 * @brief    登录判断
 * @author		wdz
 * @bug
 */

 function  log(){

	$("#log").click(function(){
		var user=$("#用户名").val();
		var code=$("#密码").val();
		var ret=/^[0-9a-zA-Z]{5,20}$/;
		
		if((ret.test(user)== 0)||(ret.test(code)== 0))
		{
			alarm_tip(1,'用户名或密码不匹配');
		}
		else 
		{
			$("#submit").click();
		}
	})

}
/*
function  log(){

	$("#log").click(function(){
		var user=$("#用户名").val();
		var code=$("#密码").val();
		var web_code=$("#web_code").val();
		var web_user=$("#web_user").val();
		
		if((user == "")||(code == ""))
		{
			alarm_tip_info_error(1,'用户名或密码不能为空');
		}
		else if((user == web_user)&&(code == web_code))
		{
			//  常规模式
			$("#index").click();
		}
		else if((user == "root")&&(code == "Hytera1993"))
		{
			//  工程模式
			$("#engineer_pattern").click();
		}
		else
		{
			alarm_tip_info_error(1,'用户名与密码不匹配');		
		}
	})

}
*/
function  log_type(flag,user,code){
	
	document.getElementById("用户名").value=user;
	document.getElementById("密码").value=code;
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
		alarm_tip(1,'用户名与密码不匹配');	
	}
}
/**
 * @brief    登录界面回车判断
 * @author		wdz
 * @bug
 */

function BindEnter(obj){     //使用document.getElementById获取到按钮对象   

	var button = document.getElementById('log');    

	if(obj.keyCode == 13)        
	{             
		button.click();            
		obj.returnValue = false;        
	} 

}

/**
 * @brief    屏蔽键值（ie有问题）
 * @author		wdz
 * @bug
 */

function Enter(event){   
   
    event=window.event||event|| event.which;;
	if(event.keyCode == 13)        
	{    
		event.returnValue=false;        
	} 
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

}



