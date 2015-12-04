var api_endpoint = "http://fojam.users.sourceforge.net/api/inputprank";

function encodeQuerySegment(str)
{
	return encodeURIComponent(str).
             replace(/%2B/gi, '+');
}

function prank_showSpinner(show)
{
	var spinner = document.getElementById("prank-spinner");
	if(show)
	{
		spinner.style.display = "block";
	}
	else
	{
		spinner.style.display = "none";
	}
}

function prank_resetInputs()
{
	var elements = document.querySelectorAll(".prank-form input, .prank-form select, .prank-form textarea");
	for(var i=0; i<elements.length; i++)
	{
		if(elements[i].nodeName=="INPUT")
		{
			elements[i].value = "";
		}
		else if(elements[i].nodeName=="SELECT")
		{
			elements[i].selectedIndex = 0;
		}
		else if(elements[i].nodeName=="TEXTAREA")
		{
			elements[i].value = "";
		}
	}
}

function prank_disableInputs(disabled)
{
	var elements = document.querySelectorAll(".prank-form input, .prank-form select, .prank-form textarea");
	for(var i=0; i<elements.length; i++)
	{
		elements[i].disabled = disabled;
	}
}

function encodeFormData(elements)
{
	var querystring = "";
	var argcount = 0;
	for(var i=0; i<elements.length; i++)
	{
		var element = elements[i];
		if(element.nodeName=="INPUT")
		{
			var name = element.name;
			if(name!=null && name!="" && typeof(name)!='undefined')
			{
				var value = element.value;
				if(argcount>0)
				{
					querystring += "&";
				}
				querystring += encodeQuerySegment(name) + "=" + encodeQuerySegment(value);
				argcount++;
			}
		}
		//TODO add support for other element types
	}
	return querystring;
}

window.addEventListener("load", function(){
	var submit = document.getElementById("prank-submit");
	submit.onclick = function(evt){
		var elements = document.querySelectorAll(".prank-form input, .prank-form select, .prank-form textarea");
		var querystring = encodeFormData(elements);
		
		prank_showSpinner(true);
		prank_disableInputs(true);
		
		var xhr = new XMLHttpRequest();
		xhr.open("POST", api_endpoint+"/postmsg.php", true);
		xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		xhr.onreadystatechange = function(){
			if(xhr.readyState==4)
			{
				if(xhr.status==200)
				{
					var response = JSON.parse(xhr.responseText);
					if(typeof(response)!='object')
					{
						if(response==true)
						{
							//success
						}
						else
						{
							window.alert("an unknown error occured");
						}
					}
					else
					{
						window.alert("error: "+response.error);
					}
				}
				else
				{
					window.alert("error "+xhr.status+": "+xhr.statusText);
				}
				
				prank_showSpinner(false);
				prank_disableInputs(false);
				prank_resetInputs();
			}
		};
		xhr.send(querystring);
	};
});
