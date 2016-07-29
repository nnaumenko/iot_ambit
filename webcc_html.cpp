/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "webcc_html.h"
#include "eeprom_config.h"

///////////////////////////////////////////////////////////////////////////////////////////
// webcc page HTML/JS code
///////////////////////////////////////////////////////////////////////////////////////////

#define FORM_URL "/setconfig"
#define FORM_METHOD "POST"

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

//HTML page body header is composed as follows:
//Part1 TitleCaption Part2 H1Caption Part3

const char PROGMEM htmlConfigBodyBegin1[] =
  "<!DOCTYPE html>" CRLF
  "<html>" CRLF
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" CRLF
  "<head>" CRLF
  "<title>"
  ;

const char PROGMEM htmlConfigBodyBegin2[] =
  "</title>" CRLF
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
  "<form method=\"" FORM_METHOD "\" action=\"" FORM_URL "\">" CRLF
  "<h1>"
  ;

const char PROGMEM htmlConfigBodyBegin3[] =
  "</h1>" CRLF
  "<div id='tab-container'>" CRLF
  ;

const char PROGMEM htmlConfigBodyEnd[] =
  "</div>" CRLF
  "</form>" CRLF
  "</body>" CRLF
  "</html>" CRLF
  ;

//Section Header is composer as follows:
//Part1 DisplayName Part2

const char PROGMEM htmlConfigSectionBegin1[] =
  "<div class=\"tab-content\">" CRLF
  "<h2 class=\"tab\">"
  ;

const char PROGMEM htmlConfigSectionBegin2[] =
  "</h2><br>" CRLF
  ;

const char PROGMEM htmlConfigSectionEnd[] =
  "</div>" CRLF
  ;

//This part is introduced before end of each section to provide "Save settings" button to each tab

const char PROGMEM htmlConfigSave[] =
  "<div class = \"cfg_save\">" CRLF
  "<input type = \"submit\" value = \"Save settings\"><br>" CRLF
  "</div>" CRLF
  ;

//Subsection Header is composer as follows:
//Part1 DisplayName Part2

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

//Checkbox parameter parts are composed as follows:
//Part1 DisplayName Part2 [TooltipBegin TooltipText TooltipEnd] Part3 InternalName Part4 InternalValue Part5
//For checked checkbox substisute Part5 with Part5Checked

const char PROGMEM htmlConfigParameterCheckboxPart1[] =
  "<div class=\"cfg_param\"><label>";

const char PROGMEM htmlConfigParameterCheckboxPart2[] =
  "</label>";

const char PROGMEM htmlConfigParameterCheckboxPart3[] =
  "<input type=\"checkbox\" name=\"";

const char PROGMEM htmlConfigParameterCheckboxPart4[] =
  "\" value=\"";

const char PROGMEM htmlConfigParameterCheckboxPart5[] =
  "\"></div>";

const char PROGMEM htmlConfigParameterCheckboxPart5Checked[] =
  "\" checked></div>";


///////////////////////////////////////////////////////////////////////////////////////////
// WebccHTML
///////////////////////////////////////////////////////////////////////////////////////////

WebccHTML::WebccHTML(Print &client) {
  this->client = &client;
}

WebccHTML::~WebccHTML() {
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  if (isWithinBody) bodyEnd();
}

void WebccHTML::bodyBegin(char * caption, boolean isCaptionProgmem) {
  if (this->client == NULL) return;
  isWithinBody = true;
  this->client->print((__FlashStringHelper *)htmlConfigBodyBegin1);
  if (caption != NULL) {
    if (isCaptionProgmem)
      this->client->print((__FlashStringHelper *)caption);
    else
      this->client->print(caption);
  }
  this->client->print((__FlashStringHelper *)htmlConfigBodyBegin2);
  if (caption != NULL) {
    if (isCaptionProgmem)
      this->client->print((__FlashStringHelper *)caption);
    else
      this->client->print(caption);
  }
  this->client->print((__FlashStringHelper *)htmlConfigBodyBegin3);
}

void WebccHTML::bodyEnd(void) {
  if (this->client == NULL) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinBody = false;
  this->client->print((__FlashStringHelper *)htmlConfigBodyEnd);
}

void WebccHTML::sectionBegin(const char * displayName, boolean isDisplayNameProgmem) {
  if (this->client == NULL) return;
  if (displayName == NULL) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinSection = true;
  this->client->print((__FlashStringHelper *)htmlConfigSectionBegin1);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigSectionBegin2);
}

void WebccHTML::sectionEnd(void) {
  if (this->client == NULL) return;
  isWithinSection = false;
  this->client->print((__FlashStringHelper *)htmlConfigSave);
  this->client->print((__FlashStringHelper *)htmlConfigSectionEnd);
}

void WebccHTML::subsectionBegin(const char * displayName, boolean isDisplayNameProgmem) {
  if (this->client == NULL) return;
  if (displayName == NULL) return;
  if (isWithinSubsection) subsectionEnd();
  isWithinSubsection = true;
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionBegin1);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionBegin2);
}

void WebccHTML::subsectionEnd(void) {
  if (this->client == NULL) return;
  isWithinSubsection = false;
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionEnd);
}

void WebccHTML::textParameter(const char * displayName, boolean isDisplayNameProgmem,
                              const char * internalName, boolean isInternalNameProgmem,
                              const char * value,
                              const char * tooltipText, boolean isTooltipTextProgmem) {
  if (this->client == NULL) return;
  if ((displayName == NULL) || (internalName == NULL)) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart1);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart2);
  if (tooltipText != NULL) {
    this->client->print((__FlashStringHelper *)htmlConfigTooltipBegin);
    if (isTooltipTextProgmem)
      this->client->print((__FlashStringHelper *)tooltipText);
    else
      this->client->print(tooltipText);
    this->client->print((__FlashStringHelper *)htmlConfigTooltipEnd);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart3);
  if (isInternalNameProgmem)
    this->client->print((__FlashStringHelper *)internalName);
  else
    this->client->print(internalName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart4);
  if (value != NULL) {
    this->client->print(value);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart5);
}

void WebccHTML::textParameter(const char * displayName, boolean isDisplayNameProgmem,
                              const char * internalName, boolean isInternalNameProgmem,
                              long value,
                              const char * tooltipText, boolean isTooltipTextProgmem) {
  const size_t maxCharsPerLongValue = 12; //max 10 digits + optional minus sign + null character
  const int decimalRadix = 10;
  char valueAsText[maxCharsPerLongValue] = {0};
  char * valueAsText2 = ltoa (value, valueAsText, decimalRadix);
  this->textParameter(displayName, isDisplayNameProgmem, internalName, isInternalNameProgmem, valueAsText2, tooltipText, isTooltipTextProgmem);
}

void WebccHTML::textParameter (const char * displayName, const char * internalName, const char * value, const char * tooltipText) {
  this->textParameter(displayName, true,
                      internalName, true,
                      value,
                      tooltipText, true);
}

void WebccHTML::textParameter (const char * displayName, const char * internalName, long value, const char * tooltipText) {
  this->textParameter(displayName, true,
                      internalName, true,
                      value,
                      tooltipText, true);
}

void WebccHTML::selectParameterBegin(const char * displayName, boolean isDisplayNameProgmem,
                                     const char * internalName, boolean isInternalNameProgmem,
                                     const char * tooltipText, boolean isTooltipTextProgmem) {
  if (this->client == NULL) return;
  if ((displayName == NULL) || (internalName == NULL)) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart1);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart2);
  if (tooltipText != NULL) {
    this->client->print((__FlashStringHelper *)htmlConfigTooltipBegin);
    if (isTooltipTextProgmem)
      this->client->print((__FlashStringHelper *)tooltipText);
    else
      this->client->print(tooltipText);
    this->client->print((__FlashStringHelper *)htmlConfigTooltipEnd);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart3);
  if (isInternalNameProgmem)
    this->client->print((__FlashStringHelper *)internalName);
  else
    this->client->print(internalName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart4);
}

void WebccHTML::selectParameterBegin(const char * displayName,
                                     const char * internalName,
                                     const char * tooltipText) {
  this->selectParameterBegin(displayName, true,
                             internalName, true,
                             tooltipText, true);
}


void WebccHTML::selectParameterOption(const char * displayName, boolean isDisplayNameProgmem, long optionValue, long actualValue) {
  if (this->client == NULL) return;
  if (displayName == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart1);
  this->client->print(optionValue, DEC);
  if (optionValue == actualValue)
    this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart2Selected);
  else
    this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart2);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart3);
}

void WebccHTML::selectParameterOption(const char * displayName,
                                      long optionValue,
                                      long actualValue) {
  this->selectParameterOption(displayName, true, optionValue, actualValue);
}

void WebccHTML::selectParameterEnd(void) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart5);
}

void WebccHTML::checkBoxParameter(const char * displayName, boolean isDisplayNameProgmem,
                                  const char * internalName, boolean isInternalNameProgmem,
                                  boolean value,
                                  const char * tooltipText, boolean isTooltipTextProgmem,
                                  long internalValue) {
  if (this->client == NULL) return;
  if ((displayName == NULL) || (internalName == NULL)) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart1);
  if (isDisplayNameProgmem)
    this->client->print((__FlashStringHelper *)displayName);
  else
    this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart2);
  if (tooltipText != NULL) {
    this->client->print((__FlashStringHelper *)htmlConfigTooltipBegin);
    if (isTooltipTextProgmem)
      this->client->print((__FlashStringHelper *)tooltipText);
    else
      this->client->print(tooltipText);
    this->client->print((__FlashStringHelper *)htmlConfigTooltipEnd);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart3);
  if (isInternalNameProgmem)
    this->client->print((__FlashStringHelper *)internalName);
  else
    this->client->print(internalName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart4);
  if (value) {
    this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart5Checked);
  }
  else {
    this->client->print((__FlashStringHelper *)htmlConfigParameterCheckboxPart5);
  }
}

void WebccHTML::checkBoxParameter(const char * displayName,
                                  const char * internalName,
                                  boolean value,
                                  const char * tooltipText,
                                  long internalValue) {
  this->checkBoxParameter(displayName, true,
                          internalName, true,
                          value,
                          tooltipText, true,
                          internalValue);
}

void WebccHTML::checkBoxParameter(const char * displayName, boolean isDisplayNameProgmem,
                                  const char * internalName, boolean isInternalNameProgmem,
                                  long value,
                                  const char * tooltipText, boolean isTooltipTextProgmem,
                                  long internalValue) {
  this->checkBoxParameter(displayName, isDisplayNameProgmem,
                          internalName, isInternalNameProgmem,
                          (boolean)(value == 0L),
                          tooltipText, isTooltipTextProgmem,
                          internalValue);
}

void WebccHTML::checkBoxParameter(const char * displayName,
                                  const char * internalName,
                                  long value,
                                  const char * tooltipText,
                                  long internalValue) {
  this->checkBoxParameter(displayName, true,
                          internalName, true,
                          value,
                          tooltipText, true,
                          internalValue);
}

//////////////////////////////////////////////////////////////////////
// WebccHTMLStringMaps
//////////////////////////////////////////////////////////////////////
void WebccHTMLStringMaps::begin (StringMapKey sectionCaption) {
  WebccHTML::bodyBegin(this->sections->find(sectionCaption), this->sections->isProgmem(sectionCaption));
}

void WebccHTMLStringMaps::end (void) {
  WebccHTML::bodyEnd();
}

void WebccHTMLStringMaps::section(StringMapKey section) {
  WebccHTML::sectionBegin(this->sections->find(section), this->sections->isProgmem(section));
}

void WebccHTMLStringMaps::subsection(StringMapKey subsection) {
  WebccHTML::subsectionBegin(this->sections->find(subsection), this->sections->isProgmem(subsection));
}


void WebccHTMLStringMaps::textParameter (StringMapKey parameter, long value) {
  WebccHTML::textParameter(this->displayNames->find(parameter), this->displayNames->isProgmem(parameter),
                           this->internalNames->find(parameter), this->internalNames->isProgmem(parameter),
                           value,
                           this->tooltips->find(parameter), this->tooltips->isProgmem(parameter));
}

void WebccHTMLStringMaps::textParameter (StringMapKey parameter, const char * value) {
  WebccHTML::textParameter(this->displayNames->find(parameter), this->displayNames->isProgmem(parameter),
                           this->internalNames->find(parameter), this->internalNames->isProgmem(parameter),
                           value,
                           this->tooltips->find(parameter), this->tooltips->isProgmem(parameter));
}

void WebccHTMLStringMaps::selectParameter(StringMapKey parameter, long value, const StringMap &options) {
  StringMapIterator optionsListIterator (options);
  WebccHTML::selectParameterBegin(this->displayNames->find(parameter), this->displayNames->isProgmem(parameter),
                                  this->internalNames->find(parameter), this->internalNames->isProgmem(parameter),
                                  this->tooltips->find(parameter), this->tooltips->isProgmem(parameter));
  optionsListIterator.first();
  while (!optionsListIterator.isDone()) {
    StringMapKey currentKey = optionsListIterator.currentKey();
    WebccHTML::selectParameterOption(optionsListIterator.currentString(),
                                     optionsListIterator.isCurrentStringProgmem(),
                                     (int)currentKey,
                                     value);
    optionsListIterator.next();
  }
  this->selectParameterEnd();
}

void WebccHTMLStringMaps::checkBoxParameter(StringMapKey parameter, boolean value, long internalValue) {
  WebccHTML::checkBoxParameter(this->displayNames->find(parameter), this->displayNames->isProgmem(parameter),
                               this->internalNames->find(parameter), this->internalNames->isProgmem(parameter),
                               value,
                               this->tooltips->find(parameter), this->tooltips->isProgmem(parameter),
                               internalValue);
}

void WebccHTMLStringMaps::checkBoxParameter(StringMapKey parameter, long value, long internalValue) {
  WebccHTML::checkBoxParameter(this->displayNames->find(parameter), this->displayNames->isProgmem(parameter),
                               this->internalNames->find(parameter), this->internalNames->isProgmem(parameter),
                               value,
                               this->tooltips->find(parameter), this->tooltips->isProgmem(parameter),
                               internalValue);
}
