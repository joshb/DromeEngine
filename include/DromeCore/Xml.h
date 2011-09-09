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

#ifndef __DROMECORE_XML_H__
#define __DROMECORE_XML_H__

#include <string>
#include <vector>
#include "Exception.h"

namespace DromeCore {

class XmlException : public Exception
{
	protected:
		unsigned int m_line;

	public:
		XmlException(unsigned int line, const char *message);
		XmlException(unsigned int line, const std::string &message);
		XmlException(const char *message);
		XmlException(const std::string &message);

		std::string toString() const;
};

class XmlAttribute
{
	protected:
		std::string m_name;
		std::string m_value;

	public:
		XmlAttribute(const char *name, const char *value);

		std::string getName() const;
		std::string getValue() const;
		std::string toString() const;
};

class XmlElement
{
	protected:
		XmlElement *m_parent;
		std::string m_name;
		std::string m_content;
		std::vector <XmlAttribute *> m_attributes;
		std::vector <XmlElement *> m_children;

	public:
		XmlElement(XmlElement *parent, const char *name = "");
		virtual ~XmlElement();

		XmlElement *getParent();

		std::string getName() const;
		void setName(const char *name);

		std::string getContent() const;
		void setContent(const char *content);

		unsigned int getNumAttributes() const;
		const XmlAttribute *getAttribute(unsigned int index) const;
		const XmlAttribute *getAttribute(const std::string &name) const;
		const XmlAttribute *getAttribute(const char *name) const;
		void addAttribute(XmlAttribute *attribute);

		unsigned int getNumChildren() const;
		const XmlElement *getChild(unsigned int index) const;
		const XmlElement *getChild(const std::string &name) const;
		const XmlElement *getChild(const char *name) const;
		void addChild(XmlElement *child);

		std::string toString(int tabLevel = 0) const;
};

class XmlTokenizer
{
	private:
		enum State
		{
			STATE_SEARCHING_FOR_TOKEN = 0,
			STATE_TAG_START_FOUND,
			STATE_COMMENT0,
			STATE_COMMENT1,
			STATE_COMMENT2,
			STATE_TOKENIZING_TAG,
			STATE_TOKENIZING_IDENTIFIER,
			STATE_TOKENIZING_QUOTED_IDENTIFIER,
			STATE_TOKENIZING_CONTENT
		};

		State m_state;
		std::string m_data;
		char m_quoteChar;

	protected:
		unsigned int m_line;

		XmlTokenizer();
		void processCharacter(char c);
		void processData(const char *data);

		virtual void tokenFound(const char *token, bool quoted = false) = 0;
};

class XmlParser : public XmlTokenizer
{
	protected:
		enum State
		{
			STATE_SEARCHING_FOR_ELEMENT = 0,
			STATE_PARSING_TAG,
			STATE_PARSING_TAG_NAME,
			STATE_PARSING_TAG_NAME_PI,
			STATE_PARSING_END_TAG_NAME,
			STATE_PARSING_ATTRIBUTES,
			STATE_PARSING_ATTRIBUTES_PI,
			STATE_PARSING_ATTRIBUTE_DELIMITER,
			STATE_PARSING_ATTRIBUTE_DELIMITER_PI,
			STATE_PARSING_ATTRIBUTE_VALUE,
			STATE_PARSING_ATTRIBUTE_VALUE_PI,
			STATE_PARSING_QUOTED_ATTRIBUTE_VALUE,
			STATE_PARSING_TAG_END,
			STATE_PARSING_CONTENT
		};

		State m_state;
		std::string m_data;

		XmlParser();

		void tokenFound(const char *token, bool quoted);

		virtual void parsedProcessingInstruction(const std::string &target) = 0;
		virtual void parsedStartTag(const std::string &tagName) = 0;
		virtual void parsedEndTag(const std::string &tagName) = 0;
		virtual void parsedAttribute(const std::string &name, const std::string &value) = 0;
		virtual void parsedContent(const std::string &content) = 0;
};

class XmlDocument : public XmlParser
{
	protected:
		XmlElement *m_rootElement;
		XmlElement *m_currentElement;

		void parsedProcessingInstruction(const std::string &target);
		void parsedStartTag(const std::string &tagName);
		void parsedEndTag(const std::string &tagName);
		void parsedAttribute(const std::string &name, const std::string &value);
		void parsedContent(const std::string &content);

	public:
		XmlDocument();
		virtual ~XmlDocument();

		void fromFile(const char *filePath);
		void fromFile(const std::string &filePath);
		void fromString(const char *s);
		void fromString(const std::string &s);

		const XmlElement *getRootElement() const;
		void setRootElement(XmlElement *value);

		std::string toString() const;
};

} // namespace DromeCore

#endif /* __DROMECORE_XML_H__ */
