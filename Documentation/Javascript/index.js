// index.js


/***********************************\ 
|*** Colored Words
\***********************************/
var colorwords_green = [
	"string", "Uint8", "Sint32", "vector",
	"SDL_Color", "SDL_Rect",
	"WindowKeyState", "WindowImage", "WindowButton", "WindowTextBox", "WindowJoystick", "WindowAudio", "WindowVideo"
];
var colorwords_blue = [
	"void", "int", "bool", "double", "template", "typename"
];
var colorwords_yellow = [
	"SDL_WINDOW_FULLSCREEN_DESKTOP", "SDL_WINDOW_FULLSCREEN", "SDL_WINDOW_SHOWN"
];
var colorwords_pink = [
	"NULL"
]

/***********************************\ 
|*** Page Funcionality 
\***********************************/
function Page_Loaded() {
	$('.sidebar_items').hide();

	// Resize page
	Page_Resized();
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
function Reload_APIS() {
	$('#content_title').hide();
	$('#content_panel').hide();
	
	$('#Window_tabs').empty().hide();
	$('#Game_tabs').empty().hide();
	$('#Network_tabs').empty().hide();
}

/***********************************\ 
|*** Helper Functions
\***********************************/
function replaceAll(str, find, replace) {
	return str.replace(new RegExp(find, 'g'), replace);
}
function colorString(str, sepvar) {
	for (var i = 0; i < colorwords_blue.length; i++) {
		var word = colorwords_blue[i];
		str = replaceAll(str, word, "<span class=\"code_color_blue\">" + word + "</span>");
	}
	for (var i = 0; i < colorwords_green.length; i++) {
		var word = colorwords_green[i];
		str = replaceAll(str, word, "<span class=\"code_color_green\">" + word + "</span>");
	}
	for (var i = 0; i < colorwords_yellow.length; i++) {
		var word = colorwords_yellow[i];
		str = replaceAll(str, word, "<span class=\"code_color_yellow\">" + word + "</span>");
	}
	for (var i = 0; i < colorwords_pink.length; i++) {
		var word = colorwords_pink[i];
		str = replaceAll(str, word, "<span class=\"code_color_pink\">" + word + "</span>");
	}
	
	var ind = 0;
	while (true) {
		var ind = str.indexOf("(", ind);
		if (ind > 0) {
			for (var i = ind; i > 0; i--) {
				if (str[i] == " ") {
					var word = str.substring(i, ind);
					
					str = replaceAll(str, word, "<span class=\"code_color_yellow\">" + word + "</span>");
					
					ind = str.indexOf("(", ind);
					break;
				}
			}
			
			ind += 1;
		} else break;
	}
	
	if (sepvar && str.indexOf("-") > 0) {
		var end = str.indexOf("-");
		var start = str.substring(0, end - 1).lastIndexOf(" ");
		
		str = str.substring(0, start) 
			+ "<span class=\"code_color_lblue\">"
			+ str.substring(start, end)
			+ "</span>"
			+ str.substring(end);
	}

	return str;
}
function createCodeSnippet(str) {
	var nstr = "";
	
	var cpiece = "";
	var fstring = false;
	var aftervar = false;
	
	for (var i = 0; i < str.length; i++) {
		var p = str[i];
		
		if (!fstring && !Number(cpiece) && (p == " " || p == "," || p == "." || p == "(")) {
			switch (cpiece) {
				case "Window":
					cpiece = ("<span class=\"code_color_green\">" + cpiece + "</span>");
					aftervar = true;
					break;
				case "window":
					cpiece = ("<span class=\"code_color_purple\">" + cpiece + "</span>");
					break;
				case "new":
					cpiece = ("<span class=\"code_color_blue\">" + cpiece + "</span>");
					break;
				// Macros
				case "SDL_WINDOW_FULLSCREEN":
				case "SDL_WINDOW_SHOWN":
					cpiece = ("<span class=\"code_color_yellow\">" + cpiece + "</span>");
					break;
				// Structs
				case "WindowAudio":
				case "WindowKeyboard":
				case "WindowMouse":
				case "WindowTextBox":
				case "WindowImage":
				case "WindowJoyStick":
				case "WindowButton":
					cpiece = ("<span class=\"code_color_green\">" + cpiece + "</span>");
					break;
				default: 
					if (p == "(") {
						cpiece = ("<span class=\"code_color_yellow\">" + cpiece + "</span>");
					}
					if (aftervar)
						cpiece = ("<span class=\"code_color_lblue\">" + cpiece + "</span>");
					else
						cpiece = ("<span class=\"code_color_grey\">" + cpiece + "</span>");
					aftervar = false;
					break;
			}
			
			nstr += (cpiece + p);
			
			cpiece = "";
		} else if (p == "\"") {
			if (fstring) {
				fstring = false;
				cpiece = "<span class=\"code_color_string\">\"" + cpiece + "\"</span>";
				
				nstr += cpiece;
				
				cpiece = "";
			} else {
				fstring = true;
				
				nstr += cpiece;
				
				cpiece = "";
			}
		} else if (!fstring && (p == "," || p == " " || p == ")")) {
			if (Number(cpiece) || Number(cpiece) == 0) {
				cpiece = "<span class=\"code_color_number\">" + cpiece + "</span>";
			}
			switch (cpiece) {
				case "SDL_WINDOW_FULLSCREEN_DESKTOP":
				case "SDL_WINDOW_FULLSCREEN":
				case "SDL_WINDOW_SHOWN":
					cpiece = ("<span class=\"code_color_yellow\">" + cpiece + "</span>");
					break;
				case "NULL":
					cpiece = ("<span class=\"code_color_pink\">" + cpiece + "</span>");
					break;
				default: break;
			}
			
			nstr += cpiece + p;
				
			cpiece = "";		
		} else {
			cpiece += p;
		}
	}
	
	nstr += cpiece;
	
	return nstr;
}
function separate_parameters(param) {
	if (param.length < 0) return "";
	if (param[0] == "None") return "";

	var ap = "";

	for (var i = 0; i < param.length; i++) {
		var dash = param[i].search("-");
		ap += param[i].substring(0, dash - 1) + ", ";
	}

	return ap.substring(0, ap.length - 2);
}
function load_properties(prop, parent, title, codesnippet, colorpredash) {
	var maindiv = "api_" + prop;
	var titlediv = "api_" + prop + "_title";
	var datadiv = "api_" + prop + "_data";
	
	$('#content_api').append(`
		<div class="api_main_div" id="` + maindiv + `">
			<div id="` + titlediv + `">
				<h2 class="api_title_text">` + title + `</h2>
			</div>
			<div class="api_data_div" id="` + datadiv + `"></div>
		</div>
	`);
	
	for (var i = 0; i < parent.length; i++) {
		var p = parent[i];
		
		if (codesnippet)
			$('#' + datadiv).append('<blockquote><h3>' + createCodeSnippet(p) + '</h3></blockquote>');
		else
			$('#' + datadiv).append('<blockquote><h3>' + colorString(p, colorpredash) + '</h3></blockquote>');
	}
}

/***********************************\ 
|*** Loading API 
\***********************************/
function load_sidebar(api, name) {
	var cparam = separate_parameters(api.Constructor.Parameters);
	var div = "#" + name + "_tabs";
	
	$(div).empty();
	$(div).show();
	
	// 
	$(div).append(`
		<div id="sidebar_cons">
			<div class="sidebar_title">
				<h2>Constructor</h2>
			</div>
			<div class="sidebar_text">
				<h3>` + name + `(` + colorString(cparam, false) + `)</h3>
			</div>
		</div>
		<div id="sidebar_func">
			<div class="sidebar_title">
				<h2>Functions</h2>
			</div>
		</div>
		<div id="sidebar_objs">
			<div class="sidebar_title">
				<h2>Objects</h2>
			</div>
		</div>
	`);
	$('.sidebar_text').click(function(){
		load_constructor(api.Constructor, name);
	});
	
	// Functions
	for (var func in api.Functions) {
		if (func.substring(0,5) == "break") continue;
		
		var f = api.Functions[func];
		var fparam = separate_parameters(f.Parameters);
		var divid = "func_" + func;
		
		$('#sidebar_func').append(`
			<div class="sidebar_text" id="` + divid + `" onclick="load_function('` + api.Name + `', '` + func + `')">
				<h3>`
					+ colorString(f.Type) + ' ' + func 
					+ '<span class="code_color_grey">(</span>'
					+ colorString(fparam) 
					+ `<span class="code_color_grey">)</span>
				</h3>
			</div>
		`);
	}
	
	// Objects - Enums
	$('#sidebar_objs').append('<div class="sidebar_subtitle"><h2>Enums</h2></div>');
	for (var en in api.Objects.EnumClasses) {
		var e = api.Objects.EnumClasses[en];
		
		$('#sidebar_objs').append(`
			<div class="sidebar_subtext" onclick="load_enum('` + api.Name + `', '` + en + `')">
				<h3><span class="code_color_lgreen">`
					+ en +
				`</span></h3>
			</div>
		`);
	}
	// Objects - Structs
	$('#sidebar_objs').append('<div class="sidebar_subtitle"><h2>Structs</h2></div>');
	for (var st in api.Objects.Structs) {
		var s = api.Objects.EnumClasses[st];
		
		$('#sidebar_objs').append(`
			<div class="sidebar_subtext" onclick="load_struct('` + api.Name + `', '` + st + `')">
				<h3><span class="code_color_lgreen">`
					+ st +
				`</span></h3>
			</div>
		`);
	}
	// Objects - Functions
	$('#sidebar_objs').append('<div class="sidebar_subtitle"><h2>Functions</h2></div>');
	for (var func in api.Objects.Functions) {
		var f = api.Objects.Functions[func];
		var fparam = separate_parameters(f.Parameters);
		
		$('#sidebar_objs').append(`
			<div class="sidebar_subtext" onclick="load_function('` + api.Name + `', '` + func + `', true)">
				<h3>`
					+ colorString(f.Type) + ' ' + func 
					+ '<span class="code_color_grey">(</span>'
					+ colorString(fparam) 
					+ `<span class="code_color_grey">)</span>
				</h3>
			</div>
		`);
	}
	// Objects - Variables
	$('#sidebar_objs').append('<div class="sidebar_subtitle"><h2>Variables</h2></div>');
	for (var va in api.Objects.Variables) {
		var v = api.Objects.Variables[va];
		
		$('#sidebar_objs').append(`
			<div class="sidebar_subtext" onclick="load_variable('` + api.Name + `', '` + va + `')">
				<h3><span class="code_color_lgreen">`
					+ va +
				`</span></h3>
			</div>
		`);
	}
}
function load_constructor(api, name) {
	var cparam = separate_parameters(api.Parameters);
	
	$('#content_api').empty();
	
	$('#content_api').append('<div class="api_top_div" id="api_constructor"></div>');
	$('#api_constructor').append('<div class="api_constructor_title"><h1>Constructor</h1></div>');
	$('#api_constructor').append('<div class="api_constructor_function"><blockquote><h3><span class=\"code_color_yellow\">' + name + '</span>(' + colorString(cparam) + ')</h3></blockquote></div>');
	
	load_properties("constructor_parameters", api.Parameters, "Parameters", false, true);
	load_properties("constructor_description", api.Description, "Description", false, false);
	load_properties("constructor_examples", api.Examples, "Examples", true, false);
}
function load_function(apiname, name, obj) {
	var api;
	if (obj) api = window[apiname].Objects.Functions[name];
	else api = window[apiname].Functions[name];
	
	var cparam = separate_parameters(api.Parameters);
	
	$('#content_api').empty();
	
	$('#content_api').append('<div class="api_top_div" id="api_functions"></div>');
	$('#api_functions').append('<div class="api_functions_title"><h1>Function</h1></div>');
	$('#api_functions').append('<div class="api_functions_function"><blockquote><h3>' + colorString(api.Type) + " <span class=\"code_color_yellow\">" + name + '</span>(' + colorString(cparam) + ')</h3></blockquote></div>');
	
	if (api.Parameters.length > 0 && api.Parameters[0] != "None") load_properties("function_parameters", api.Parameters, "Parameters", false, true);
	if (api.Returns.length > 0 && api.Returns[0] != "Nothing") load_properties("function_returns", api.Returns, "Returns", false, false);
	if (api.Description.length > 0 && api.Description[0] != "None") load_properties("function_description", api.Description, "Description", false, false);
	if (api.Examples.length > 0 && api.Examples[0] != "None") load_properties("function_examples", api.Examples, "Examples", true, false);
}
function load_enum(apiname, name) {
	var obj = window[apiname].Objects;
	
	$('#content_api').empty();
	
	$('#content_api').append('<div class="api_top_div" id="api_objects"></div>');
	$('#api_objects').append('<div class="api_objects_title"><h1>Enum</h1></div>');
	$('#api_objects').append('<div class="api_objects_function"><blockquote><h3><span class=\"code_color_blue\">enum </span><span class=\"code_color_lgreen\">' + name + '</span></h3></blockquote></div>');
	
	load_properties("enum_values", obj.EnumClasses[name], "Values", false, false);
}
function load_struct(apiname, name) {
	var func = window[apiname].Objects.Structs;
	var api = func[name];
	
	$('#content_api').empty();
	
	$('#content_api').append('<div class="api_top_div" id="api_objects"></div>');
	$('#api_objects').append('<div class="api_objects_title"><h1>Struct</h1></div>');
	$('#api_objects').append('<div class="api_objects_function"><blockquote><h3><span class=\"code_color_green\">struct </span><span class=\"code_color_lgreen\">' + name + '</span></h3></blockquote></div>');
	
	load_properties("object_struct", api.Variables, "Variables", false, true);
	load_properties("object_function", api.Functions, "Functions", false, true);
}
function load_variable(apiname, name) {
	var obj = window[apiname].Objects;
	
	$('#content_api').empty();
	
	$('#content_api').append('<div class="api_top_div" id="api_objects"></div>');
	$('#api_objects').append('<div class="api_objects_title"><h1>Variable</h1></div>');
	$('#api_objects').append('<div class="api_objects_function"><blockquote><h3>' + colorString(obj.Variables[name].Type) + ' <span class=\"code_color_lblue\">' + name + '</span></h3></blockquote></div>');
	
	load_properties("variable_description", obj.Variables[name].Description, "Description", false, false);
}

/***********************************\ 
|*** Main Functions
\***********************************/
function Display_Window_API() {
	Reload_APIS();
	
	load_constructor(window_api.Constructor, "Window");
	load_sidebar(window_api, "Window");
}
function Display_Game_API() {
	Reload_APIS();
	
	load_constructor(game_api.Constructor, "Game");
	load_sidebar(game_api, "Game");
}