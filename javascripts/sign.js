/*********************************************************************************
------2012.08.01  v0.0------
签到发布
------2012.08.04  v0.1------
更新1:修正第一个贴吧不能签到的bug
------2012.08.22  v0.2------
更新1:解决I贴吧布局改变后不能签到的情况
------2012.08.30  v0.3------
更新1:优化签到时间，签过和不能签的改成等待1秒...
更新2:签到的贴吧显示签到的名次
更新3:签到的名次用红色的字符显示
------2012.09.01  v0.4------
更新1:修正图片类贴吧不能签到的情况
更新2:修改建议吧友的链接为新开标签打开
更新3:修改在有背景的贴吧点开本签到时，背景为贴吧背景看不清字的情况
------2012.09.02  v0.5------
更新1:添加转向各贴吧的链接，方便签到失败时候手动签到
------2012.09.03  v0.6------
更新1:增加被封禁贴吧显示异常的提示
------2012.09.25  v0.7------
更新1:增加对几种签到错误的现象正常显示并重复签到至成功
------2012.10.15  v0.8------
更新1:修复由于百度更新导致的收集贴吧失败问题
更新2:增加在页面顶部的完成提示
更新3:增加贴吧序号，方便查看签到进度
------2012.10.15  v0.9------
更新1:修复由于百度更新导致的收集贴吧失败问题
------2012.11.14  v0.9 patch by misakamm------
更新1:修正c#，%等有奇怪符号贴吧签到不成功的bug
*********************************************************************************/
document.body.style.fontFamily ="微软雅黑";
document.body.style.fontSize ="10pt";
document.body.style.background ="#ffffff";
document.body.innerHTML="";

var ajax=false;
var rewords="";
var ba;
var rt="ok";
var bai;
var bas=[];
var myself="";

if(window.XMLHttpRequest){//非IE，包括（IE7，IE8）
	ajax=new XMLHttpRequest();
}
else if(window.ActiveXObject){//IE
	var versions=['Microsoft.XMLHTTP', 'MSXML.XMLHTTP', 'Msxml2.XMLHTTP.7.0','Msxml2.XMLHTTP.6.0','Msxml2.XMLHTTP.5.0', 'Msxml2.XMLHTTP.4.0', 'MSXML2.XMLHTTP.3.0', 'MSXML2.XMLHTTP'];
	for(var i=0; i<versions.length; i++){
			try{
				ajax=new ActiveXObject(versions[i]);
			}
			catch(e){
				ajax=false;
			}
	}
}
if(ajax==false){
	document.body.innerHTML+="遇到未知问题，程序不能继续下去!";
}
else{
	document.body.innerHTML+="请自行检查有没有签到正确，如有错误，欢迎反馈<a href='http://tieba.baidu.com/p/1768731534'  target='_blank'>蝉曦吧反馈专用贴</a><br>";
	
	
	// document.body.innerHTML+="<span style='color:red;'>代码修改中，期间使用本签到可能出现故障，请稍候再用</span><br>";
	document.body.innerHTML+="<span style='color:red;'>特别注意:请在自动签到的时候，放慢你的发帖和回复速度，防止应为操作过频被度受和谐ID。</span><br>";
	document.body.innerHTML+="<span style='color:red;'>------2012.10.15  v0.7------</span><br>";
	
	document.body.innerHTML+="<span style='color:red;'>更新1:修复由于百度更新导致的收集贴吧失败问题</span><br>";
	document.body.innerHTML+="<span style='color:red;'>特别提示:请不要在i贴吧页面使用本签到，请随便到那个贴吧的版面使用，并确保已登录！</span><br>";
	document.body.innerHTML+="<span style='color:red;'>------------------------------</span><br>";
	
	document.body.innerHTML+="<span style='color:red;'>备注1:由于本代码放置于免费空间，最近因为访问量变大，导致空间里出现了广告，所以，大家不用再本站的首页留言板留言，有问题请到本人贴吧反馈专用贴咨询</span><br>";
	document.body.innerHTML+="<span style='color:red;'>备注2:请转载的童鞋去掉我的个人贴吧信息，以免引起误会</span><br>";
	document.body.innerHTML+="<span style='color:red;'>备注3:感谢以下吧友建议</span><a href=\"/i/sys/jump?un=%C8%F6%B5%A9%D2%B2%C0%E4\" target=\"_blank\">@撒旦也冷</a> | <a href=\"/i/sys/jump?un=wangxi3799\" target=\"_blank\">@wangxi3799</a> | <a href=\"/i/sys/jump?un=%D6%BF%B0%AE%B3%C9%A4%CE%DD%E6\" target=\"_blank\">@挚爱成の萱</a> | <a href=\"/i/sys/jump?un=%D3%D0%D6%BB%D0%A1F%D8%BC\" target=\"_blank\">@有只小F丶</a>  <br>";

	document.body.innerHTML+="程序开始工作...<br>---开始收集I贴吧中的贴吧名，请等待 ......<br>";
	whoIam()//开始运行
}
function urlreplace(urlStr) {
	return encodeURIComponent(urlStr);
}
function checksigned(urlStr){

	bai++;
	if(bai>ba.length){
		document.body.innerHTML+="<br>签到完毕！请自行检查有没有签到正确，如有错误，欢迎反馈<a href='http://tieba.baidu.com/p/1768731534'  target='_blank'>蝉曦吧反馈专用贴</a><br><br><br><br><br>";
		
		document.body.innerHTML="<br><span style='color:red;font-size: 25pt;'>签到完毕！</span> <br>"+document.body.innerHTML;
		return 0;
	}else{
		document.body.innerHTML+="<br>+"+(bai-1)+" .<a href=\"http://tieba.baidu.com/f?kw="+urlreplace(ba[bai-1])+"&ie=utf-8&tp=0\" target=\"_blank\">"+ba[bai-1]+"吧</a>";
	}
	
	ajax.onreadystatechange=function(){
		if(ajax.readyState == 4){
			if(ajax.status == 200){
				rewords=ajax.responseText;
				var aa=cutchar(rewords,"<span class=\"sign_index_num j_signin_index\">","</span>");
				var tbs=cutchar(rewords,"PageData.tbs = \"","\";");
				
				if(cutchar(rewords,"PageData.user.is_block = ",";//是否已封禁")=="1"){
					document.body.innerHTML+="--您在本吧<span style='color:red;'>被封禁，不能签到!</span>";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					setTimeout("checksigned(\""+ba[bai]+"\");",1000);
					return 0;	
				}
				
				if(rewords.indexOf("<span class=\"sign_index_num j_signin_index\">")<0){
					document.body.innerHTML+="--本吧还没开放签到系统!";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					setTimeout("checksigned(\""+ba[bai]+"\");",1000);
					return 0;	
				}					
				if(aa=="0"){
					document.body.innerHTML+="--还没签到!";
					signed(ba[bai-1],tbs);
					//alert(rt);
					if(rt=="ok")	var  waittime=6000;
					else if(rt=="err0")	var  waittime=6000;
					else 	var  waittime=6000;
					setTimeout("checksigned(\""+ba[bai]+"\");",waittime);
				}
				else{
					document.body.innerHTML+="--本吧已签到!";
					document.body.innerHTML+="--今日,你是本吧第 <span style='color:red;'>"+aa+"</span> 个签到!";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					setTimeout("checksigned(\""+bas[bai]+"\");",1000);
				}
				return 0;
			}
		}
	};
	if(ajax){
		ajax.open("get", "http://tieba.baidu.com/f?kw="+urlreplace(ba[bai-1])+"&ie=utf-8&tp=0");
		ajax.send(null);
	}else{
		ajax.open("get", "http://tieba.baidu.com/f?kw="+urlreplace(ba[bai-1])+"&ie=utf-8&tp=0", true);
		ajax.send();
	}
}

function signed(urlStr,tbs){
	ajax.onreadystatechange=function(){
		if(ajax.readyState == 4){
			if(ajax.status == 200){
				rewords=ajax.responseText;
				var ranks=cutchar(rewords,"\"user_sign_rank\":",",");
				if(ranks.length<6){
					document.body.innerHTML+="--签到完毕,您是第 <span style='color:red;'>"+ranks+"</span> 个签到的...";
					document.body.innerHTML+="---防止度受和谐，等待6s ing...<br>";
					rt =  "ok";
				}
				else if(ranks=="r\":\"\\u60a8\\u5c1a\\u5728\\u9ed1\\u540d\\u5355\\u4e2d\\uff0c\\u4e0d\\u80fd\\u64cd\\u4f5c\\u3002\""){
					document.body.innerHTML+="--<span style='color:red;'>您尚在黑名单中，不能操作.....</span> ";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					rt =  "err00";
				}
				else if(anks=="r\":\"\\u5927\\u5bb6\\u90fd\\u5728\\u62a2\\u7b2c1\\uff0c\\u51fa\\u624b\\u6162\\u4e86\\u70b9\\uff0c\\u518d\\u7b7e\\u4e00\\u6b21\\u8bd5\\u8bd5\""){
					document.body.innerHTML+="--<span style='color:red;'>大家都在抢第1，出手慢了点，再签一次试试</span> --正在尝试重新签到...";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					bai--;
					rt =  "err01";
				}
				else if(anks=="r\":\"\\u7b7e\\u5230\\u592a\\u9891\\u7e41\\u4e86\\u70b9\\uff0c\\u4f11\\u606f\\u7247\\u523b\\u518d\\u6765\\u5427\""){
					document.body.innerHTML+="--<span style='color:red;'>签到太频繁了点，休息片刻再来吧...</span> --正在尝试重新签到...";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					bai--;
					rt =  "err02";
				}
				else {
					document.body.innerHTML+="--签到失败，请将以下红色的错误信息以文字的方式反馈到专贴中,以便得到解决，谢谢你的支持： <span style='color:red;'>"+ranks+"</span> ";
					document.body.innerHTML+="---防止度受和谐，等待1s ing...<br>";
					rt =  "err";
				}
			}
		}
	};
	

	document.body.innerHTML+="--开始签到...";
	ajax.open("post","http://tieba.baidu.com/sign/add");
	ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	ajax.send("kw="+urlreplace(urlStr)+"&ie=utf-8&tbs="+tbs);

}

//查贴吧数量
function countba(){
	ajax.onreadystatechange=function(){
		if(ajax.readyState == 4){
			if(ajax.status == 200){
				rewords=ajax.responseText;
				ba=rewords.split("<a class=\"j_ba_link"); 
				
				for(var i=1;i<ba.length;i++){
					bas[i-1]=cutchar(ba[i],"forum=\"","\"");
					var aa=cutchar(ba[i],">","</a>");
					if(ba[i].indexOf("text_point\"")>=0)
						var aa=cutchar(ba[i],"title=\"","\"");
					ba[i]=aa;	
				}
				document.body.innerHTML+="估测完毕:您喜欢的贴吧有"+(ba.length-1)+"个，最长将花费时间"+parseInt(ba.length*6/60)+"分"+(ba.length*6%60)+"秒";
				
				bai=1;
				checksigned(bas[bai]);
			}
		}
	};
	if(ajax){
		ajax.open("get", "http://tieba.baidu.com/i/"+myself);
		ajax.send(null);
	}else{
		ajax.open("get", "http://tieba.baidu.com/i/"+myself, true);
		ajax.send();
	}
}


//我是谁
function whoIam(){
	ajax.onreadystatechange=function(){
		if(ajax.readyState == 4){
			if(ajax.status == 200){
				rewords=ajax.responseText;
				myself=cutchar(rewords,"\"itieba_id\":",",");
				countba();
			}
		}
	};
	if(ajax){
		ajax.open("get", "http://tieba.baidu.com/f?kw=%B2%F5%EA%D8");
		ajax.send(null);
	}else{
		ajax.open("get", "http://tieba.baidu.com/f?kw=%B2%F5%EA%D8", true);
		ajax.send();
	}
}		
		
function cutchar(allstr,prechar,endchar){
	var preposition=allstr.indexOf(prechar)+prechar.length;
	var strlength=allstr.indexOf(endchar,preposition);
	return allstr.substring(preposition,strlength);
}

function isCon(arr, val){
	for(var i=0; i<arr.length; i++){
		if(arr[i] == val)
			return i-1;
	}
	return 0;
}

function changeStr(allstr,cbit,changeStr){ 
	  return allstr.substring(0,cbit)+changeStr+allstr.substring(cbit+1,allstr.length); 
   }

