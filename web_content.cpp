/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "web_content.h"

#define HTML_FORMATTING

#ifdef HTML_FORMATTING
#define TAB "\t"
#define CRLF "\r\n"
#define CSSCOMMENT(comment) "/* " comment " */"
#define JSCOMMENT(comment) "// " comment
#else
#define TAB ""
#define CRLF ""
#define CSSCOMMENT(comment) ""
#define JSCOMMENT(comment) ""
#endif

const char PROGMEM httpStatusCode200[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char PROGMEM httpStatusCode404[] = "HTTP/1.1 404 Not Found\r\n";
const char PROGMEM httpStatusCode405[] = "HTTP/1.1 405 Method Not Allowed\r\n";

const char PROGMEM htmlConfigBodyBegin[] =
  "<!DOCTYPE html>" CRLF
  "<html>" CRLF
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" CRLF
  "<head>" CRLF
  "<title>Module configuration</title>" CRLF
  "<style type=\"text/css\" media=\"all\">" CRLF
  "html{overflow-y:scroll;}" CRLF
  "body{font-family:Arial;background-color:#D0FFD0;color:#000000;font-size:85%;}" CRLF
  "h1{text-align:center;font-size:130%;font-weight:bold;}" CRLF
  "h2{text-align:center;font-size:120%;font-weight:bold;margin-top:2em;}" CRLF
  "h3{text-align:center;font-size:110%;font-weight:bold;}" CRLF
  "h3:first-child{margin-top:-.25em;}" CRLF
  "input, select {display:block;text-align:left;float:right;}" CRLF
  ".cfg_subsection {text-align:left;" CRLF
  TAB "margin:auto;" CRLF
  TAB "margin-bottom:1em;" CRLF
  TAB "width:25em;" CRLF
  TAB "padding:1em;" CRLF
  TAB "background-color:#66FF66;" CRLF
  TAB "border-style:solid;" CRLF
  TAB "border-color:black;" CRLF
  TAB "border-width:2px;" CRLF
  TAB "overflow:auto;" CRLF
  "}" CRLF
  ".cfg_save {" CRLF
  TAB "text-align:left;" CRLF
  TAB "margin:auto;" CRLF
  TAB "margin-bottom:1em;" CRLF
  TAB "width:25em;" CRLF
  TAB "padding:1em;" CRLF
  TAB "background-color:#AAFFAA;" CRLF
  TAB "border-style:solid;" CRLF
  TAB "border-color:black;" CRLF
  TAB "border-width:2px;" CRLF
  TAB "overflow:auto;" CRLF
  TAB "}" CRLF
  ".cfg_param {overflow:auto;padding-bottom:0.25em;}" CRLF
  CSSCOMMENT ("Tooltip styles") CRLF
  ".tooltip_ref:hover .tooltip_text {display:block;}" CRLF
  ".tooltip_ref {color:#006000;font-weight:bold;font-size:60%;}" CRLF
  ".tooltip_text {" CRLF
  TAB "display:none;" CRLF
  TAB "background:#A0FFA0;" CRLF
  TAB "margin-left:1em;" CRLF
  TAB "padding:1em;" CRLF
  TAB "position:absolute;" CRLF
  TAB "color:black;" CRLF
  TAB "font-size:75%;" CRLF
  TAB "tmax-width:20em;" CRLF
  TAB "line-height:100%;" CRLF
  "}" CRLF
  CSSCOMMENT ("Tabs styles") CRLF
  ".tab-content {border-top:1px solid #A0A0A0;margin:auto;width:37em;}" CRLF
  ".tab-header {position:relative;left:0.75em;height:3em;width:37em;margin:auto;margin-bottom:0px;padding-bottom:0px;}" CRLF
  ".tab-header ul.tab-list {margin:0;padding:0;position:absolute;bottom:-1px;width:37em;}" CRLF
  ".tab-header ul.tab-list li {display:inline;list-style:none;margin:0;}" CRLF
  ".tab-header ul.tab-list a,.tab-header ul.tab-list span,.tab-header ul.tab-list a.tab-active {" CRLF
  TAB "width:8em;" CRLF
  TAB "display:block;" CRLF
  TAB "float:left;" CRLF
  TAB "padding:4px 0;" CRLF
  TAB "margin:1px 2px 0 0;" CRLF
  TAB "text-align:center;" CRLF
  TAB "font-size:85%;" CRLF
  TAB "text-decoration:none;" CRLF
  "}" CRLF
  ".tab-header ul.tab-list span,.tab-header ul.tab-list a.tab-active {" CRLF
  TAB "border:1px solid #606060;" CRLF
  TAB "border-bottom:none;" CRLF
  TAB "background:#66FF66;" CRLF
  TAB "padding-bottom:7px;" CRLF
  TAB "margin-top:0;" CRLF 
  "}" CRLF
  ".tab-header ul.tab-list a {" CRLF
  TAB "\tbackground:#D0D0D0;"
  TAB "border-top:1px solid #A0A0A0;" CRLF
  TAB "border-left:1px solid #A0A0A0;" CRLF
  TAB "border-right:1px solid #A0A0A0;" CRLF
  TAB "border-bottom:none;" CRLF
  TAB "color:black;" CRLF
  "}" CRLF
  ".tab {display: none;}" CRLF
  "</style>" CRLF
  "<script type=\"text/javascript\">" CRLF
  "/*<![CDATA[*/" CRLF
  CSSCOMMENT ("HTML Tabs begin") CRLF
  CRLF
  "function getChildElementsByClassName(parentElement,className){"  CRLF
  TAB "var i,childElements,pattern,result;" CRLF
  TAB "result=new Array();" CRLF
  TAB "pattern=new RegExp(\"\\\\b\"+className+\"\\\\b\");" CRLF
  CRLF
  TAB "childElements=parentElement.getElementsByTagName('*');" CRLF
  TAB "for(i=0;i<childElements.length;i++){" CRLF
  TAB TAB "if(childElements[i].className.search(pattern)!=-1){" CRLF
  TAB TAB TAB "result[result.length]=childElements[i];" CRLF
  TAB TAB "}" CRLF
  TAB "}" CRLF
  TAB "return result;" CRLF
  "}" CRLF
  CRLF
  CRLF
  "function BuildTabs(containerId){" CRLF
  TAB "var i,tabContainer,tabContents,tabHeading,title,tabElement;" CRLF
  TAB "var divElement,ulElement,liElement,tabLink,linkText;" CRLF
  CRLF
  TAB JSCOMMENT("assume that if document.getElementById exists, then this will work...") CRLF
  TAB "if(!eval('document.getElementById'))return;" CRLF
  TAB "tabContainer=document.getElementById(containerId);" CRLF
  TAB "if(tabContainer==null)return;" CRLF
  TAB "tabContents=getChildElementsByClassName(tabContainer,'tab-content');" CRLF
  TAB "if(tabContents.length==0)return;" CRLF
  CRLF
  TAB "divElement=document.createElement(\"div\");" CRLF
  TAB "divElement.className='tab-header';" CRLF
  TAB "divElement.id=containerId+'-header';" CRLF
  TAB "ulElement=document.createElement(\"ul\");" CRLF
  TAB "ulElement.className='tab-list';" CRLF
  CRLF
  TAB "tabContainer.insertBefore(divElement,tabContents[0]);" CRLF
  TAB "divElement.appendChild(ulElement);" CRLF
  CRLF
  TAB "for(i=0;i<tabContents.length;i++){" CRLF
  TAB TAB "tabHeading=getChildElementsByClassName(tabContents[i],'tab');" CRLF
  TAB TAB "title=tabHeading[0].childNodes[0].nodeValue;" CRLF
  CRLF
  TAB TAB JSCOMMENT("create the tabs as an unsigned list") CRLF
  TAB TAB "liElement=document.createElement(\"li\");" CRLF
  TAB TAB "liElement.id=containerId+'-tab-'+i;" CRLF
  CRLF
  TAB TAB "tabLink=document.createElement(\"a\");" CRLF
  TAB TAB "linkText=document.createTextNode(title);" CRLF
  CRLF
  TAB TAB "tabLink.className=\"tab-item\";" CRLF
  CRLF
  TAB TAB "tabLink.setAttribute(\"href\",\"javascript://\");" CRLF
  TAB TAB "tabLink.onclick=new Function (\"ActivateTab('\"+containerId+\"', \"+i+\")\");" CRLF
  CRLF
  TAB TAB "ulElement.appendChild(liElement);" CRLF
  TAB TAB "liElement.appendChild(tabLink);" CRLF
  TAB TAB "tabLink.appendChild(linkText);" CRLF
  CRLF
  TAB TAB JSCOMMENT ("remove the H1") CRLF
  TAB "tabContents[i].removeChild;" CRLF
  TAB "}" CRLF
  "}" CRLF
  CRLF
  CRLF
  "function ActivateTab(containerId,activeTabIndex){" CRLF
  TAB "var i,tabContainer,tabContents;" CRLF
  CRLF
  TAB "tabContainer=document.getElementById(containerId);" CRLF
  TAB "if(tabContainer==null) return;" CRLF
  CRLF
  TAB "tabContents=getChildElementsByClassName(tabContainer, 'tab-content');" CRLF
  TAB "if(tabContents.length>0) {" CRLF
  TAB TAB "for(i=0;i<tabContents.length;i++) {" CRLF
/*  TAB TAB TAB "//tabContents[i].className = \"tab-content\";" CRLF*/
  TAB TAB TAB "tabContents[i].style.display=\"none\";" CRLF
  TAB TAB "}" CRLF
  CRLF
  TAB TAB "tabContents[activeTabIndex].style.display=\"block\";" CRLF
  CRLF
  TAB TAB "tabList=document.getElementById(containerId+'-list');" CRLF
  TAB TAB "tabs=getChildElementsByClassName(tabContainer,'tab-item');" CRLF
  TAB TAB "if(tabs.length>0) {" CRLF
  TAB TAB TAB "for(i=0;i<tabs.length;i++){" CRLF
  TAB TAB TAB TAB "tabs[i].className=\"tab-item\";" CRLF
  TAB TAB TAB "}" CRLF
  CRLF
  TAB TAB TAB "tabs[activeTabIndex].className=\"tab-item tab-active\";" CRLF
  TAB TAB TAB "tabs[activeTabIndex].blur();" CRLF
  TAB TAB "}" CRLF
  TAB "}" CRLF
  "}" CRLF
  CRLF
  CRLF
  "function InitTabs(){" CRLF
  TAB "BuildTabs('tab-container');" CRLF
  TAB "ActivateTab('tab-container',0);" CRLF
  "}" CRLF
  CRLF
  CSSCOMMENT ("HTML Tabs end") CRLF
  CRLF
  "/*]]>*/" CRLF
  "</script>" CRLF
  "</head>" CRLF
  "<body onload=\"InitTabs();\">" CRLF
  "<h1>Module configuration</h1>" CRLF
  "<div id='tab-container'>" CRLF
  ;

const char PROGMEM htmlConfigBodyEnd[] =
  "</div>" CRLF
  "</body>" CRLF
  "</html>" CRLF
  ;

const char PROGMEM htmlConfigSectionBegin1[] =
  "<div class=\"tab-content\">" CRLF
  "<h2 class=\"tab\">"
  ;

const char PROGMEM htmlConfigSectionBegin2[] =
  "</h2><br>" CRLF
  "<form method=\"GET\" action=\"/\">" CRLF
  ;

const char PROGMEM htmlConfigSectionEnd[] =
  "</form>" CRLF
  "</div>" CRLF
  ;

const char PROGMEM htmlConfigSubsectionBegin1[] =
  "<div class=\"cfg_subsection\">" CRLF
  "<h3>"
  ;

const char PROGMEM htmlConfigSubsectionBegin2[] =
  "</h3>" CRLF
  ;

const char PROGMEM htmlConfigSubsectionEnd[] =
  "</div>" CRLF
  ;

const char PROGMEM htmlConfigSave[] =
  "<div class = \"cfg_save\">" CRLF
  "<input type = \"submit\" value = \"Save settings\"><br>" CRLF
  "</div>" CRLF
  ;

const char PROGMEM htmlConfigTooltipBegin[] =
  "&nbsp;<span class=\"tooltip_ref\">[?]<span class=\"tooltip_text\">";

const char PROGMEM htmlConfigTooltipEnd[] =
  "</span></span>";

//Text parameter parts are composed as follows:
//Part1 DisplayName Part2 [TooltipBegin TooltipText TooltipEnd] Part3 InternalName Part4 Value Part5

const char PROGMEM htmlConfigParameterTextPart1[] =
  "<div class=\"cfg_param\"><label>";

const char PROGMEM htmlConfigParameterTextPart2[] =
  "</label>";

const char PROGMEM htmlConfigParameterTextPart3[] =
  "<input type=\"text\" name=\"";

const char PROGMEM htmlConfigParameterTextPart4[] =
  "\" value=\"";

const char PROGMEM htmlConfigParameterTextPart5[] =
  "\"></div>" CRLF;

//Select parameter parts are composed as follows:
//Part1 DisplayName Part2 [TooltipBegin TooltipText TooltipEnd] Part3 InternalName Part4 OptionList Part5
//An option in option list is composed as follows:
//Part1 OptionValue Part2 DisplayName Part3
//For currently selected option substisute Part2 with Part2Selected

const char PROGMEM htmlConfigParameterSelectPart1[] =
  "<div class=\"cfg_param\"><label>";

const char PROGMEM htmlConfigParameterSelectPart2[] =
  "</label>" CRLF;

const char PROGMEM htmlConfigParameterSelectPart3[] =
  "<select name=\"";

const char PROGMEM htmlConfigParameterSelectPart4[] =
  "\">" CRLF;

const char PROGMEM htmlConfigParameterSelectPart5[] =
  "</select></div>";

const char PROGMEM htmlConfigParameterSelectOptionPart1[] =
  "<option value=\"";

const char PROGMEM htmlConfigParameterSelectOptionPart2[] =
  "\">";

const char PROGMEM htmlConfigParameterSelectOptionPart2Selected[] =
  "\" selected>";

const char PROGMEM htmlConfigParameterSelectOptionPart3[] =
  "</option>" CRLF;
