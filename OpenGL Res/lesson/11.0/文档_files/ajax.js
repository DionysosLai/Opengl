<!--
function AJAX()
{
	this.http_request = false;	
	this.CreateXMLHttp = function()
	{
		var xmlhttp = false;
		if (window.XMLHttpRequest) { // Mozilla, Safari,...
			xmlhttp = new XMLHttpRequest();
		} else if (window.ActiveXObject) { // IE
			try {
				xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
					xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {}
			}
		}
		if (!xmlhttp) {
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}
		return xmlhttp;
	}

	this.request = function(url) {
		this.http_request = this.CreateXMLHttp();
		this.http_request.onreadystatechange = this.callback;
		this.http_request.open('GET', url, true);
		this.http_request.send(null);
	}

	this.callback = function() {}
}

function SetExPoint(id,name,point) 
{
	if (!confirm("确定给分吗？"))
	{
		return false;
	}
	var ajax = new AJAX();
	var obj = document.getElementById("re"+id);
	var retstr;
	var url =  "bbsaction.asp?action=isgood&id=" + id + "&point=" + point;
	var content= obj.innerHTML;
	ajax.callback = function()
	{
		obj.innerHTML = "<img src='image/loading.gif' /> 评分处理中";
		if (ajax.http_request.readyState == 4) 
		{
			if (ajax.http_request.status == 200) 
			{
					if (!isNaN(ajax.http_request.responseText))
					{
						obj.innerHTML = '<span style="color:#FF00FF">&nbsp;<b>此帖被评' + ajax.http_request.responseText + '分</b></span>';
					}
					else
					{
						alert(ajax.http_request.responseText);
						obj.innerHTML = content;
					}
			}
			else
			{
				alert('There was a problem with the request.');
			}
		}
  	};
	ajax.request(url);
}


function Up(id) 
{
	var ajax = new AJAX();
	var retstr;
	var url =  "bbsaction.asp?action=up&id=" + id;
	ajax.callback = function()
	{
		document.getElementById("upnum").innerHTML = "<img src='image/loading.gif' />";
		var upnum = document.getElementById("upnum").innerHTML;
		if (ajax.http_request.readyState == 4) {
			if (ajax.http_request.status == 200) {
					switch(parseInt(ajax.http_request.responseText)){
						case 0:
							document.getElementById("upnum").innerHTML = upnum;
							alert("您已推荐过此帖");
							break;
						case -1:
							document.getElementById("upnum").innerHTML = upnum;
							alert("您尚未登陆");
							break;
						default:
							document.getElementById("upnum").innerHTML = ajax.http_request.responseText;
							document.getElementById("uptext").innerHTML = "已推荐";
							break;
					}
			}
			else
				alert('There was a problem with the request.');
		}
  	};
	ajax.request(url);
}
//-->