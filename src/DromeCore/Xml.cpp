/*
 * Copyright (C) 2010 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <DromeCore/String.h>
#include <DromeCore/Xml.h>

using namespace std;

namespace DromeCore {

static const char *ENTITIES[][2] = {
	{ "&nbsp;", " " },
	{ "&amp;", "&" },
	{ "&apos;", "'" },
	{ "&lt;", "<" },
	{ "&gt;", ">" },
	{ "&quot;", "\"" }
};

static const int NUM_ENTITIES = sizeof(ENTITIES) / sizeof(ENTITIES[0]);

static bool
isWhitespace(const char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

static string
decodeString(const string &s)
{
	size_t start;
	size_t pos = 0;
	string str = s;

	// look for xml entities in the string
	while((start = str.find('&', pos)) != string::npos) {
		size_t end = start + 1;

		pos = end;

		// find end of entity
		while((str[end] >= 'a' && str[end] <= 'z') ||
		      (str[end] >= 'A' && str[end] <= 'Z') ||
		      (str[end] >= '0' && str[end] <= '9') ||
		      str[end] == '#' || str[end] == 'x')
			++end;

		// last character must be a semicolon
		if(str[end] != ';')
			continue;

		size_t len = end - start + 1;

		// loop through known entities
		for(int i = 0; i < NUM_ENTITIES; ++i) {
			const char **entity = ENTITIES[i];

			// compare entity in string to known entity
			if(str.compare(start, len, entity[0]) == 0) {
				// replace entity in string with its corresponding character
				str.replace(start, len, entity[1]);
				break;
			}
		}
	}

	return str;
}

static string
encodeString(const string &s)
{
	string str = s;

	// loop through each character in the string
	for(size_t i = 0; i < str.length(); ++i) {
		// loop through each known entity except for nbsp (index 0 in ENTITIES)
		for(int j = 1; j < NUM_ENTITIES; ++j) {
			const char **entity = ENTITIES[j];

			// compare characters
			if(str[i] == entity[1][0]) {
				// replace character with its corresponding entity
				str.replace(i, 1, entity[0]);
				i += strlen(entity[0]) - 1;
			}
		}
	}

	return str;
}

static string
trimString(const string &s)
{
	size_t len;
	string str = s;

	// remove whitespace from the beginning
	while((len = str.length()) != 0 && isWhitespace(str[0]))
		str.erase(0, 1);

	// remove whitespace from the end
	while((len = str.length()) != 0 && isWhitespace(str[len - 1]))
		str.erase(len - 1, 1);

	return str;
}

/*
 * XmlException class
 */
XmlException::XmlException(unsigned int line, const char *message)
 : Exception(message)
{
	m_line = line;
}

XmlException::XmlException(unsigned int line, const string &message)
 : Exception(message)
{
	m_line = line;
}

XmlException::XmlException(const char *message)
 : Exception(message)
{
	m_line = 0;
}

XmlException::XmlException(const string &message)
 : Exception(message)
{
	m_line = 0;
}

string
XmlException::toString() const
{
	if(m_line != 0) {
		stringstream line;
		line << m_line;

		return m_message + " (line " + line.str() + ")";
	} else {
		return m_message;
	}
}

/*
 * XmlAttribute class
 */
XmlAttribute::XmlAttribute(const char *name, const char *value)
{
	m_name = name;
	m_value = value;
}

string
XmlAttribute::getName() const
{
	return m_name;
}

string
XmlAttribute::getValue() const
{
	return m_value;
}

string
XmlAttribute::toString() const
{
	return m_name + "=\"" + encodeString(m_value) + "\"";
}

/*
 * XmlElement class
 */
XmlElement::XmlElement(XmlElement *parent, const char *name)
{
	m_parent = parent;
	m_name = name;
}

XmlElement::~XmlElement()
{
	// delete attributes
	for(unsigned int i = 0; i < m_attributes.size(); ++i)
		delete m_attributes[i];

	// delete children
	for(unsigned int i = 0; i < m_children.size(); ++i)
		delete m_children[i];
}

XmlElement *
XmlElement::getParent()
{
	return m_parent;
}

string
XmlElement::getName() const
{
	return m_name;
}

void
XmlElement::setName(const char *name)
{
	m_name = name;
}

string
XmlElement::getContent() const
{
	return m_content;
}

void
XmlElement::setContent(const char *content)
{
	m_content = content;
}

unsigned int
XmlElement::getNumAttributes() const
{
	return (unsigned int)m_attributes.size();
}

const XmlAttribute *
XmlElement::getAttribute(unsigned int index) const
{
	if(index >= m_attributes.size())
		throw XmlException(String("XmlElement::getAttribute(): Invalid index ") + index);

	return m_attributes[index];
}

const XmlAttribute *
XmlElement::getAttribute(const string &name) const
{
	for(unsigned int i = 0; i < m_attributes.size(); ++i) {
		if(name == m_attributes[i]->getName())
			return m_attributes[i];
	}

	return NULL;
}

const XmlAttribute *
XmlElement::getAttribute(const char *name) const
{
	return getAttribute(string(name));
}

void
XmlElement::addAttribute(XmlAttribute *attribute)
{
	m_attributes.insert(m_attributes.end(), attribute);
}

unsigned int
XmlElement::getNumChildren() const
{
	return (unsigned int)m_children.size();
}

const XmlElement *
XmlElement::getChild(unsigned int index) const
{
	if(index >= m_children.size())
		throw XmlException(String("XmlElement::getChild(): Invalid index ") + index);

	return m_children[index];
}

const XmlElement *
XmlElement::getChild(const string &name) const
{
	for(unsigned int i = 0; i < m_children.size(); ++i) {
		if(name == m_children[i]->getName())
			return m_children[i];
	}

	return NULL;
}

const XmlElement *
XmlElement::getChild(const char *name) const
{
	return getChild(string(name));
}

void
XmlElement::addChild(XmlElement *child)
{
	m_children.insert(m_children.end(), child);
}

string
XmlElement::toString(int tabLevel) const
{
	string str;

	for(int i = 0; i < tabLevel; ++i)
		str += '\t';

	// if the element has no name, it's a text data
	// element and its content can just be returned
	if(m_name.length() == 0) {
		str += encodeString(m_content);
		return str;
	}

	str += "<" + m_name;

	// append attributes to string
	for(unsigned int i = 0; i < m_attributes.size(); ++i)
		str += " " + m_attributes[i]->toString();

	// append children to string
	if(m_children.size() != 0) {
		str += ">";

		for(unsigned int i = 0; i < m_children.size(); ++i)
			str += '\n' + m_children[i]->toString(tabLevel + 1);

		str += '\n';
		for(int i = 0; i < tabLevel; ++i)
			str += '\t';
		str += "</" + m_name + ">";
	} else {
		str += " />";
	}

	return str;
}

/*
 * XmlTokenizer class
 */
XmlTokenizer::XmlTokenizer()
{
	m_state = STATE_SEARCHING_FOR_TOKEN;
	m_line = 1;
}

void
XmlTokenizer::processCharacter(char c)
{
	// increment line count if this
	// is a newline character
	if(c == '\n')
		++m_line;

	switch(m_state) {
		default:
		case STATE_SEARCHING_FOR_TOKEN:
			// skip whitespace characters
			if(isWhitespace(c))
				break;

			// start of tag found
			if(c == '<') {
				m_state = STATE_TAG_START_FOUND;
				break;
			}

			// start of content found
			m_state = STATE_TOKENIZING_CONTENT;
			m_data = c;
			break;

		case STATE_COMMENT0:
		case STATE_COMMENT1:
			// expecting "-"
			if(c == '-') {
				if(m_state == STATE_COMMENT0) {
					m_state = STATE_COMMENT1;
				} else {
					m_state = STATE_COMMENT2;
					// STATE_COMMENT2 requires m_data to be 3 characters long
					m_data = "   ";
				}
				break;
			}

			throw XmlException(m_line, "XmlTokenizer::processCharacter(): Invalid comment format - expected \"-\"");

		case STATE_COMMENT2:
			m_data = (m_data + c).substr(1);

			// end of comment
			if(m_data == "-->") {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				m_data = "";
			}

			break;

		case STATE_TAG_START_FOUND:
			// start of comment
			if(c == '!') {
				m_state = STATE_COMMENT0;
				break;
			}

			// start of another tag type - emit the "<"
			// token and go to the STATE_TOKENIZING_TAG case
			m_state = STATE_TOKENIZING_TAG;
			tokenFound("<");

		case STATE_TOKENIZING_TAG:
			// skip whitespace characters
			if(isWhitespace(c))
				break;

			// single character tokens
			if(c == '/' || c == '!' || c == '?' || c == '=') {
				m_data = c;
				tokenFound(m_data.c_str());
				break;
			}

			// end of tag found
			if(c == '>') {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				tokenFound(">");
				break;
			}

			// start of quoted identifier
			if(c == '"' || c == '\'') {
				m_state = STATE_TOKENIZING_QUOTED_IDENTIFIER;
				m_data.clear();
				m_quoteChar = c;
				break;
			}

			// start of non-quoted identifier
			m_state = STATE_TOKENIZING_IDENTIFIER;
			m_data = c;
			break;

		case STATE_TOKENIZING_QUOTED_IDENTIFIER:
			// end of quoted identifier
			if(c == m_quoteChar) {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str(), true);
				break;
			}

			m_data += c;
			break;

		case STATE_TOKENIZING_IDENTIFIER:
			// whitespace ends identifier
			if(isWhitespace(c)) {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str());
				break;
			}

			// single character tokens
			if(c == '/' || c == '!' || c == '?' || c == '=') {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str());
				m_data = c;
				tokenFound(m_data.c_str());
				break;
			}

			// end of tag found
			if(c == '>') {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				tokenFound(m_data.c_str());
				tokenFound(">");
				break;
			}

			m_data += c;
			break;

		case STATE_TOKENIZING_CONTENT:
			// start of tag found
			if(c == '<') {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(trimString(m_data).c_str());
				tokenFound("<");
				break;
			}

			m_data += c;
			break;
	}
}

void
XmlTokenizer::processData(const char *data)
{
	size_t len = strlen(data);
	for(size_t i = 0; i < len; ++i)
		processCharacter(data[i]);
}

/*
 * XmlParser class
 */
XmlParser::XmlParser()
{
	m_state = STATE_SEARCHING_FOR_ELEMENT;
}

void
XmlParser::tokenFound(const char *token, bool quoted)
{
#ifdef XMLDEBUG
	cout << "STATE: " << m_state << endl;
	cout << "TOKEN: " << token << endl;
#endif /* XMLDEBUG */

	switch(m_state) {
		default:
		case STATE_SEARCHING_FOR_ELEMENT:
			// if this is a tag, start searching for the name;
			// otherwise, this is content
			if(strcmp(token, "<") == 0)
				m_state = STATE_PARSING_TAG;
			else
				parsedContent(string(token));

			break;

		case STATE_PARSING_TAG:
			// processing instruction
			if(strcmp(token, "?") == 0) {
				m_state = STATE_PARSING_TAG_NAME_PI;
				break;
			}

			// end tag
			if(strcmp(token, "/") == 0) {
				m_state = STATE_PARSING_END_TAG_NAME;
				break;
			}

			// start tag
			m_state = STATE_PARSING_ATTRIBUTES;
			parsedStartTag(string(token));
			break;

		case STATE_PARSING_TAG_NAME:
		case STATE_PARSING_TAG_NAME_PI:
		case STATE_PARSING_END_TAG_NAME:
			// name must be non-quoted
			if(quoted)
				throw XmlException(m_line, "XmlParser::tokenFound(): Expected non-quoted tag name");

			// name must not be a special single-character token
			if(strcmp(token, "<") == 0 || strcmp(token, "!") == 0 ||
			   strcmp(token, "?") == 0 || strcmp(token, "/") == 0 ||
			   strcmp(token, ">") == 0)
				throw XmlException(m_line, "XmlParser::tokenFound(): Expected tag name");

			if(m_state == STATE_PARSING_TAG_NAME) {
				m_state = STATE_PARSING_ATTRIBUTES;
				parsedStartTag(string(token));
			} else if(m_state == STATE_PARSING_TAG_NAME_PI) {
				m_state = STATE_PARSING_ATTRIBUTES_PI;
				parsedProcessingInstruction(string(token));
			} else {
				m_state = STATE_PARSING_TAG_END;
				parsedEndTag(string(token));
			}

			break;

		case STATE_PARSING_ATTRIBUTES:
			// if this is a slash, this is a tag with no content or children
			if(strcmp(token, "/") == 0) {
				m_state = STATE_PARSING_TAG_END;
				parsedEndTag(string(""));
				break;
			}

			// end of attributes; search for content/children
			if(strcmp(token, ">") == 0) {
				m_state = STATE_SEARCHING_FOR_ELEMENT;
				break;
			}

			m_state = STATE_PARSING_ATTRIBUTE_DELIMITER;
			m_data = token;
			break;

		case STATE_PARSING_ATTRIBUTES_PI:
			// if this is a question mark, there are no more attributes
			if(strcmp(token, "?") == 0) {
				m_state = STATE_PARSING_TAG_END;
				break;
			}

			m_state = STATE_PARSING_ATTRIBUTE_DELIMITER_PI;
			m_data = token;
			break;

		case STATE_PARSING_ATTRIBUTE_DELIMITER:
		case STATE_PARSING_ATTRIBUTE_DELIMITER_PI:
			// must be an equals sign
			if(strcmp(token, "=") == 0) {
				if(m_state == STATE_PARSING_ATTRIBUTE_DELIMITER)
					m_state = STATE_PARSING_ATTRIBUTE_VALUE;
				else
					m_state = STATE_PARSING_ATTRIBUTE_VALUE_PI;
				break;
			}

			throw XmlException(m_line, (string("XmlParser::tokenFound(): Expected '=', got '") + token + "'").c_str());
			break;

		case STATE_PARSING_ATTRIBUTE_VALUE:
			m_state = STATE_PARSING_ATTRIBUTES;
			parsedAttribute(m_data, string(token));
			break;

		case STATE_PARSING_ATTRIBUTE_VALUE_PI:
			m_state = STATE_PARSING_ATTRIBUTES_PI;
			parsedAttribute(m_data, string(token));
			break;

		case STATE_PARSING_TAG_END:
			// this must be the end of the tag
			if(strcmp(token, ">") == 0) {
				m_state = STATE_SEARCHING_FOR_ELEMENT;
				break;
			}

			throw XmlException(m_line, (string("XmlParser::tokenFound(): Expected '>', got '") + token + "'").c_str());
			break;
	}
}

/*
 * XmlDocument class
 */
XmlDocument::XmlDocument()
{
	m_rootElement = NULL;
	m_currentElement = NULL;
}

XmlDocument::~XmlDocument()
{
	if(m_rootElement != NULL)
		delete m_rootElement;
}

void
XmlDocument::parsedProcessingInstruction(const string &/*target*/)
{
}

void
XmlDocument::parsedStartTag(const string &tagName)
{
	XmlElement *element = new XmlElement(m_currentElement, tagName.c_str());

	// if we don't have a root element yet, this must be the root;
	// otherwise add it as a child to the current element
	if(!m_rootElement)
		m_rootElement = element;
	else
		m_currentElement->addChild(element);

	m_currentElement = element;
}

void
XmlDocument::parsedEndTag(const string &tagName)
{
	if(!m_currentElement)
		throw XmlException(m_line, "XmlDocument::parsedEndTag(): Got end tag with no current element");

	if(tagName.length() != 0 && tagName.compare(m_currentElement->getName()) != 0)
		throw XmlException(m_line, "XmlDocument::parsedEndTag(): Got end tag with invalid name");

	// end tag is valid, so we're done with the
	// current element and can go back to the parent
	m_currentElement = m_currentElement->getParent();
}

void
XmlDocument::parsedAttribute(const string &name, const string &value)
{
	if(!m_currentElement) {
		//throw XmlException("XmlDocument::parsedAttribute(): Got attribute with no current element");
		return;
	}

	XmlAttribute *attribute = new XmlAttribute(name.c_str(), decodeString(value).c_str());
	m_currentElement->addAttribute(attribute);
}

void
XmlDocument::parsedContent(const string &content)
{
	if(!m_currentElement)
		throw XmlException(m_line, "XmlDocument::parsedContent(): Got content with no current element");

	XmlElement *element = new XmlElement(m_currentElement);
	element->setContent(content.c_str());

	m_currentElement->addChild(element);
}

void
XmlDocument::fromFile(const char *filePath)
{
	if(m_rootElement != NULL)
		delete m_rootElement;
	m_rootElement = NULL;
	m_currentElement = NULL;

	// open file for reading
	ifstream in(filePath);
	if(in.is_open() == false)
		throw XmlException("XmlDocument::fromFile(): Unable to open file for reading");

	// read and process each line
	while(in.eof() == false) {
		string line;
		getline(in, line);
		line += '\n';
		processData(line.c_str());
	}

	in.close();
}

void
XmlDocument::fromFile(const std::string &filePath)
{
	fromFile(filePath.c_str());
}

void
XmlDocument::fromString(const char *s)
{
	if(m_rootElement != NULL)
		delete m_rootElement;
	m_rootElement = NULL;
	m_currentElement = NULL;

	processData(s);
}

void
XmlDocument::fromString(const std::string &s)
{
	fromString(s.c_str());
}

const XmlElement *
XmlDocument::getRootElement() const
{
	return m_rootElement;
}

void
XmlDocument::setRootElement(XmlElement *value)
{
	m_rootElement = value;
}

string
XmlDocument::toString() const
{
	if(!m_rootElement)
		return string("");

	return string("<?xml version=\"1.0\"?>\n") + m_rootElement->toString();
}

} // namespace DromeCore
