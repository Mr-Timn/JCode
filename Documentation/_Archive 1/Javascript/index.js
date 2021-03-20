// index.js

var colorwords = [ "int", "double", "string", "SDL_Color" ];
var colormap = new Map();
colormap.set("int", "code_color_blue");
colormap.set("double", "code_color_blue");
colormap.set("string", "code_color_green");
colormap.set("SDL_Color", "code_color_green");


function replaceAll(str, find, replace) {
	return str.replace(new RegExp(find, 'g'), replace);
}
function spanVariable(str) {
	for (var i = 0; i < colorwords.length; i++) {
		var word = colorwords[i];
		str = replaceAll(str, word, "<span class=\"" + colormap.get(word) + "\">" + word + "</span>");
	}

	return str;
}

function Page_Loaded() {
	// Resize page
	Page_Resized();
	Display_Window_API();
}
function Page_Resized() {
	$('#sidebar').css('height', $(document.body).height());

	$('#header').css('left', $('#sidebar').width());
	$('#header').css('width', $(document.body).width() - $('#sidebar').width());

	$('#content').css('left', $('#sidebar').width());
	$('#content').css('width', $(document.body).width() - $('#sidebar').width());
	$('#content').css('height', $(document.body).height() - ($('#header').height() + $('#footer').height()));

	$('#footer').css('left', parseInt($('#sidebar').width()) + parseInt($(document.body).css('margin')));
	$('#footer').css('width', $(document.body).width()- $('#sidebar').width());
}

function separate_parameters(param) {
	if (param.length < 0) return "";
	if (param[0] == "None") return "";

	var ap = "";

	for (var i = 0; i < param.length; i++) {
		var dash = param[i].search("-");
		ap += param[i].substring(0, dash - 1) + ", ";
	}

	return ap;
}
function load_properties_toggle(type, data, parent) {
	if (data[0] == "None" || data[0] == "Nothing") return;

	var cont = "api_" + parent + "_" + type.toLowerCase();
	var cont_title = type.toLowerCase() + "_title_" + parent;
	var cont_toggle = type.toLowerCase() + "_toggle_" + parent;
	var cont_text = type.toLowerCase() + "_text_" + parent;

	// Create container
	$('#api_' + parent).append('<div class="info_api_containter" id="' + cont + '"></div>');

	// Populate paramaters
	$('#' + cont).append('<div class="info_title" id="' + cont_title + '"></div>');
	$("#" + cont).append('<div class="info_toggle" id="' + cont_toggle + '"></div>');
	$('#' + cont).append('<div class="info_text" id="' + cont_text + '"></div>')

	$('#' + cont_title).append('<b><h2>' + type + ': </h2></b>');
	$('#' + cont_toggle).append('<button>Hidden</button>');

	for (var i = 0; i < data.length; i++) {
		var dash = data[i].search("-");
		$('#' + cont_text).append('<blockquote><p><span style="color: #00f0e4;">' + data[i].substring(0, dash) + '</span><b>' + data[i].substring(dash) + '</b></p>');
	}

	// Add param toggle effect
	$('#' + cont_text).hide();
	$('#' + cont_toggle).click(function() {
		if($('#' + cont_text).is(":hidden")) {
			$('#' + cont_text).show();
			$('button', this).text("Shown");
		} else {
			$('#' + cont_text).hide();
			$('button', this).text('Hidden');
		}
	});
}
function Display_Window_API() {
	// Hide main page content
	$('#content_title').hide();
	$('#content_panel').hide();

	// Populate function
	var fparam = separate_parameters(window_api.Constructor.Parameters);
	$('#api_constructor_function').append('<blockquote><h2>window(' + spanVariable(fparam.substring(0, fparam.length - 2)) + ')</h2>');
	// Create paramaters
	load_properties_toggle("Parameters", window_api.Constructor.Parameters, "constructor");
	// Create description
	load_properties_toggle("Description", window_api.Constructor.Description, "constructor");
	// Create examples
	load_properties_toggle("Examples", window_api.Constructor.Examples, "constructor");

	for (var func in window_api.Functions) {
		if (func.substring(0,5) == "break") {
			continue;
		}

		var fid = "api_function_" + func;

		$('#api_functions_list').append('<div id="' + fid + '"></div>');
		//$('#' + fid).append('<div class="api_function_title"><blockquote><h2>' + func + '</h2></div>');

		var fparam = separate_parameters(window_api.Functions[func].Parameters);
		$('#' + fid).append('<blockquote><div class="api_function_title"><h2>window.' + func + '(' + spanVariable(fparam.substring(0, Math.max(0, fparam.length - 2))) + ')</h2></div>');

		load_properties_toggle("Parameters",  window_api.Functions[func].Parameters,  "function_" + func);
		load_properties_toggle("Description", window_api.Functions[func].Description, "function_" + func);
		load_properties_toggle("Examples",    window_api.Functions[func].Examples,    "function_" + func);
	}
}
