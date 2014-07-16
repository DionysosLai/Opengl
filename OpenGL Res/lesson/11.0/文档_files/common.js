<!--
/*
var currentpos,timer;
function sc()
{
	clearInterval(timer);
}

function initialize()
{
	timer=setInterval("scrollwindow()",10);
}

function scrollwindow()
{
	currentpos=document.body.scrollTop;window.scroll(0,++currentpos);
}

//document.onmousedown=sc
//document.ondblclick=initialize
*/
var newWindow=null
function subwin(loadpos,target,width,height)
{
	if(!newWindow||newWindow.closed)
	{
		newWindow=window.open(loadpos,target,"resizable,scrollbars,dependent,width="+width+",height="+height);
	}
	else
	{
		newWindow.focus();
	}
}

function check()
{
	if(document.frmedit.content.value==""){
	alert("请输入内容");
	return false;
	}
	if(document.frmedit.content.value.length>5000){
	alert("内容超过5000字，请分割发送");
	document.frmedit.content.focus();
	return false;
	}
	document.frmedit.Submit.value = "提交中请稍候";
	document.frmedit.Submit.disabled=true;
	return true;
	document.form.submit();
}

function SaveAs(ID)
{
	if(document.execCommand)
	{
		var savewindow=window.open("showtxt.asp?id=" + ID);
		savewindow.document.execCommand('SaveAs',true,'Pfan_'+ID+'.htm');
		savewindow.close();
	}
	else
		alert('SaveAs Function Not Supported by Browser');
}

function movebbs(id)
{
	document.movebbs.submit();
}

function bbsaction(type)
{
	switch(type)
	{
		case 1: document.bbsaction.action.value="urgency";break;
		case 2: document.bbsaction.action.value="lock";break;
		case 3: document.bbsaction.action.value="over";break;
		case 4: document.bbsaction.action.value="updatetime";break;
		default:break;
	}
	document.bbsaction.submit();
}

function bbsmanage(action)
{
	document.bbsmanage.action.value = action;
	document.bbsmanage.submit();
}

function jump(id,page)
{
	location.href="post-"+id+"-"+page+".html";
}

function ResizeImage(img){
	if (img.width>500)
	{
		scale = img.width / img.height;
		img.width = 500;
		img.height = 500 / scale;
	}
}

//-->