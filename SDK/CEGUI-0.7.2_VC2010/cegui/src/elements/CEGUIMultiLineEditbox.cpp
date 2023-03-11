/***********************************************************************
	filename: 	CEGUIMultiLineEditbox.cpp
	created:	30/6/2004
	author:		Paul D Turner

	purpose:	Implementation of the Multi-line edit box base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "elements/CEGUIMultiLineEditbox.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUITextUtils.h"
#include "CEGUIImage.h"
#include "CEGUIExceptions.h"
#include "CEGUICoordConverter.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String MultiLineEditbox::EventNamespace("MultiLineEditbox");
const String MultiLineEditbox::WidgetTypeName("CEGUI/MultiLineEditbox");


/*************************************************************************
    MultiLineEditboxWindowRenderer
*************************************************************************/
MultiLineEditboxWindowRenderer::MultiLineEditboxWindowRenderer(const String& name) :
    WindowRenderer(name, MultiLineEditbox::EventNamespace)
{
}

//----------------------------------------------------------------------------//
void MultiLineEditboxWindowRenderer::onLookNFeelAssigned()
{
    assert(d_window != 0);

    // ensure window's text has a terminating \n
    String text(d_window->getText());
    if (text.empty() || text[text.length() - 1] != '\n')
    {
        text.append(1, '\n');
        d_window->setText(text);
    }
}

/*************************************************************************
	TODO:

	Clipboard support
	Undo support
*************************************************************************/
/*************************************************************************
	Static Properties for this class
*************************************************************************/
MultiLineEditboxProperties::ReadOnly				MultiLineEditbox::d_readOnlyProperty;
MultiLineEditboxProperties::WordWrap				MultiLineEditbox::d_wordWrapProperty;
MultiLineEditboxProperties::CaratIndex				MultiLineEditbox::d_caratIndexProperty;
MultiLineEditboxProperties::SelectionStart			MultiLineEditbox::d_selectionStartProperty;
MultiLineEditboxProperties::SelectionLength			MultiLineEditbox::d_selectionLengthProperty;
MultiLineEditboxProperties::MaxTextLength			MultiLineEditbox::d_maxTextLengthProperty;
MultiLineEditboxProperties::SelectionBrushImage     MultiLineEditbox::d_selectionBrushProperty;
MultiLineEditboxProperties::ForceVertScrollbar      MultiLineEditbox::d_forceVertProperty;


/*************************************************************************
	Constants
*************************************************************************/
// event names
const String MultiLineEditbox::EventReadOnlyModeChanged( "ReadOnlyChanged" );
const String MultiLineEditbox::EventWordWrapModeChanged( "WordWrapModeChanged" );
const String MultiLineEditbox::EventMaximumTextLengthChanged( "MaximumTextLengthChanged" );
const String MultiLineEditbox::EventCaratMoved( "CaratMoved" );
const String MultiLineEditbox::EventTextSelectionChanged( "TextSelectionChanged" );
const String MultiLineEditbox::EventEditboxFull( "EditboxFullEvent" );
const String MultiLineEditbox::EventVertScrollbarModeChanged( "VertScrollbarModeChanged" );
const String MultiLineEditbox::EventHorzScrollbarModeChanged( "HorzScrollbarModeChanged" );

// Static data initialisation
String MultiLineEditbox::d_lineBreakChars("\n");

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String MultiLineEditbox::VertScrollbarNameSuffix( "__auto_vscrollbar__" );
const String MultiLineEditbox::HorzScrollbarNameSuffix( "__auto_hscrollbar__" );

/*************************************************************************
	Constructor for the MultiLineEditbox base class.
*************************************************************************/
MultiLineEditbox::MultiLineEditbox(const String& type, const String& name) :
	Window(type, name),
	d_readOnly(false),
	d_maxTextLen(String::max_size()),
	d_caratPos(0),
	d_selectionStart(0),
	d_selectionEnd(0),
	d_dragging(false),
	d_dragAnchorIdx(0),
	d_wordWrap(true),
	d_widestExtent(0.0f),
	d_forceVertScroll(false),
	d_forceHorzScroll(false),
	d_selectionBrush(0)
{
	addMultiLineEditboxProperties();

    // override default and disable text parsing
    d_textParsingEnabled = false;
}


/*************************************************************************
	Destructor for the MultiLineEditbox base class.
*************************************************************************/
MultiLineEditbox::~MultiLineEditbox(void)
{
}


/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void MultiLineEditbox::initialiseComponents(void)
{
	// create the component sub-widgets
	Scrollbar* vertScrollbar = getVertScrollbar();
	Scrollbar* horzScrollbar = getHorzScrollbar();

    vertScrollbar->subscribeEvent(
        Window::EventShown, Event::Subscriber(
            &MultiLineEditbox::handle_vertScrollbarVisibilityChanged, this));

    vertScrollbar->subscribeEvent(
        Window::EventHidden, Event::Subscriber(
            &MultiLineEditbox::handle_vertScrollbarVisibilityChanged, this));

    vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&MultiLineEditbox::handle_scrollChange, this));
    horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&MultiLineEditbox::handle_scrollChange, this));

	formatText(true);
	performChildWindowLayout();
}


/*************************************************************************
	return true if the edit box has input focus.
*************************************************************************/
bool MultiLineEditbox::hasInputFocus(void) const
{
	return isActive();
}


/*************************************************************************
	return the current selection start point.
*************************************************************************/
size_t MultiLineEditbox::getSelectionStartIndex(void) const
{
	return (d_selectionStart != d_selectionEnd) ? d_selectionStart : d_caratPos;
}


/*************************************************************************
	return the current selection end point.
*************************************************************************/
size_t MultiLineEditbox::getSelectionEndIndex(void) const
{
	return (d_selectionStart != d_selectionEnd) ? d_selectionEnd : d_caratPos;
}


/*************************************************************************
	return the length of the current selection (in code points / characters).
*************************************************************************/
size_t MultiLineEditbox::getSelectionLength(void) const
{
	return d_selectionEnd - d_selectionStart;
}


/*************************************************************************
	Specify whether the edit box is read-only.
*************************************************************************/
void MultiLineEditbox::setReadOnly(bool setting)
{
	// if setting is changed
	if (d_readOnly != setting)
	{
		d_readOnly = setting;
		WindowEventArgs args(this);
 		onReadOnlyChanged(args);
	}

}


/*************************************************************************
	Set the current position of the carat.
*************************************************************************/
void MultiLineEditbox::setCaratIndex(size_t carat_pos)
{
	// make sure new position is valid
    if (carat_pos > getText().length() - 1)
	{
       carat_pos = getText().length() - 1;
	}

	// if new position is different
	if (d_caratPos != carat_pos)
	{
		d_caratPos = carat_pos;
		ensureCaratIsVisible();

		// Trigger "carat moved" event
		WindowEventArgs args(this);
		onCaratMoved(args);
	}

}


/*************************************************************************
	Define the current selection for the edit box
*************************************************************************/
void MultiLineEditbox::setSelection(size_t start_pos, size_t end_pos)
{
	// ensure selection start point is within the valid range
    if (start_pos > getText().length() - 1)
	{
       start_pos = getText().length() - 1;
	}

	// ensure selection end point is within the valid range
    if (end_pos > getText().length() - 1)
	{
       end_pos = getText().length() - 1;
	}

	// ensure start is before end
	if (start_pos > end_pos)
	{
		size_t tmp = end_pos;
		end_pos = start_pos;
		start_pos = tmp;
	}

	// only change state if values are different.
	if ((start_pos != d_selectionStart) || (end_pos != d_selectionEnd))
	{
		// setup selection
		d_selectionStart = start_pos;
		d_selectionEnd	 = end_pos;

		// Trigger "selection changed" event
		WindowEventArgs args(this);
		onTextSelectionChanged(args);
	}

}


/*************************************************************************
	set the maximum text length for this edit box.
*************************************************************************/
void MultiLineEditbox::setMaxTextLength(size_t max_len)
{
	if (d_maxTextLen != max_len)
	{
		d_maxTextLen = max_len;

		// Trigger max length changed event
		WindowEventArgs args(this);
		onMaximumTextLengthChanged(args);

		// trim string
        if (getText().length() > d_maxTextLen)
		{
            String newText = getText();
            newText.resize(d_maxTextLen);
            setText(newText);

			onTextChanged(args);
		}

	}

}


/*************************************************************************
	Scroll the view so that the current carat position is visible.
*************************************************************************/
void MultiLineEditbox::ensureCaratIsVisible(void)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

	// calculate the location of the carat
	Font* fnt = getFont();
	size_t caratLine = getLineNumberFromIndex(d_caratPos);

	if (caratLine < d_lines.size())
	{
		Rect textArea(getTextRenderArea());

		size_t caratLineIdx = d_caratPos - d_lines[caratLine].d_startIdx;

		float ypos = caratLine * fnt->getLineSpacing();
        float xpos = fnt->getTextExtent(getText().substr(d_lines[caratLine].d_startIdx, caratLineIdx));

		// adjust position for scroll bars
		xpos -= horzScrollbar->getScrollPosition();
		ypos -= vertScrollbar->getScrollPosition();

		// if carat is above window, scroll up
		if (ypos < 0)
		{
			vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + ypos);
		}
		// if carat is below the window, scroll down
		else if ((ypos += fnt->getLineSpacing()) > textArea.getHeight())
		{
			vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + (ypos - textArea.getHeight()) + fnt->getLineSpacing());
		}

		// if carat is left of the window, scroll left
		if (xpos < 0)
		{
			horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + xpos - 50);
		}
		// if carat is right of the window, scroll right
		else if (xpos > textArea.getWidth())
		{
			horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + (xpos - textArea.getWidth()) + 50);
		}

	}

}


/*************************************************************************
	Set whether the text will be word wrapped or not.
*************************************************************************/
void MultiLineEditbox::setWordWrapping(bool setting)
{
	if (setting != d_wordWrap)
	{
		d_wordWrap = setting;
		formatText(true);

		WindowEventArgs args(this);
		onWordWrapModeChanged(args);
	}

}


/*************************************************************************
	display required integrated scroll bars according to current state
	of the edit box and update their values.
*************************************************************************/
void MultiLineEditbox::configureScrollbars(void)
{
    Scrollbar* const vertScrollbar = getVertScrollbar();
    Scrollbar* const horzScrollbar = getHorzScrollbar();
    const float lspc = getFont()->getLineSpacing();

    //
    // First show or hide the scroll bars as needed (or requested)
    //
    // show or hide vertical scroll bar as required (or as specified by option)
    if (d_forceVertScroll ||
        (static_cast<float>(d_lines.size()) * lspc > getTextRenderArea().getHeight()))
    {
        vertScrollbar->show();

        // show or hide horizontal scroll bar as required (or as specified by option)
        horzScrollbar->setVisible(d_forceHorzScroll ||
                                  (d_widestExtent > getTextRenderArea().getWidth()));
    }
    // show or hide horizontal scroll bar as required (or as specified by option)
    else if (d_forceHorzScroll ||
             (d_widestExtent > getTextRenderArea().getWidth()))
    {
        horzScrollbar->show();

        // show or hide vertical scroll bar as required (or as specified by option)
        vertScrollbar->setVisible(d_forceVertScroll ||
            (static_cast<float>(d_lines.size()) * lspc > getTextRenderArea().getHeight()));
    }
    else
    {
        vertScrollbar->hide();
        horzScrollbar->hide();
    }

	//
	// Set up scroll bar values
	//
	Rect renderArea(getTextRenderArea());

	vertScrollbar->setDocumentSize(static_cast<float>(d_lines.size()) * lspc);
	vertScrollbar->setPageSize(renderArea.getHeight());
	vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
	vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());

	horzScrollbar->setDocumentSize(d_widestExtent);
	horzScrollbar->setPageSize(renderArea.getWidth());
	horzScrollbar->setStepSize(ceguimax(1.0f, renderArea.getWidth() / 10.0f));
	horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}


/*************************************************************************
	Format the text into lines as needed by the current formatting options.
*************************************************************************/
void MultiLineEditbox::formatText(void)
{
    formatText(true);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::formatText(const bool update_scrollbars)
{
    // TODO: ASSAF - todo
	// clear old formatting data
	d_lines.clear();
	d_widestExtent = 0.0f;

	String paraText;

	Font* fnt = getFont();

	if (fnt)
	{
		float areaWidth = getTextRenderArea().getWidth();

		String::size_type	currPos = 0;
		String::size_type	paraLen;
		LineInfo	line;

        while (currPos < getText().length())
		{
           if ((paraLen = getText().find_first_of(d_lineBreakChars, currPos)) == String::npos)
			{
               paraLen = getText().length() - currPos;
			}
			else
			{
				++paraLen -= currPos;
			}

            paraText = getText().substr(currPos, paraLen);

			if (!d_wordWrap || (areaWidth <= 0.0f))
			{
				// no word wrapping, so we are just one long line.
				line.d_startIdx = currPos;
				line.d_length	= paraLen;
				line.d_extent	= fnt->getTextExtent(paraText);
				d_lines.push_back(line);

				// update widest, if needed.
				if (line.d_extent > d_widestExtent)
				{
					d_widestExtent = line.d_extent;
				}

			}
			// must word-wrap the paragraph text
			else
			{
				String::size_type lineIndex = 0;

				// while there is text in the string
				while (lineIndex < paraLen)
				{
					String::size_type  lineLen = 0;
					float lineExtent = 0.0f;

					// loop while we have not reached the end of the paragraph string
					while (lineLen < (paraLen - lineIndex))
					{
						// get cp / char count of next token
						size_t nextTokenSize = getNextTokenLength(paraText, lineIndex + lineLen);

						// get pixel width of the token
						float tokenExtent  = fnt->getTextExtent(paraText.substr(lineIndex + lineLen, nextTokenSize));

						// would adding this token would overflow the available width
						if ((lineExtent + tokenExtent) > areaWidth)
						{
							// Was this the first token?
							if (lineLen == 0)
							{
								// get point at which to break the token
								lineLen = fnt->getCharAtPixel(paraText.substr(lineIndex, nextTokenSize), areaWidth);
							}

							// text wraps, exit loop early with line info up until wrap point
							break;
						}

						// add this token to current line
						lineLen    += nextTokenSize;
						lineExtent += tokenExtent;
					}

					// set up line info and add to collection
					line.d_startIdx = currPos + lineIndex;
					line.d_length	= lineLen;
					line.d_extent	= lineExtent;
					d_lines.push_back(line);

					// update widest, if needed.
					if (lineExtent > d_widestExtent)
					{
						d_widestExtent = lineExtent;
					}

					// update position in string
					lineIndex += lineLen;
				}

			}

			// skip to next 'paragraph' in text
			currPos += paraLen;
		}

	}

    if (update_scrollbars)
        configureScrollbars();

	invalidate();
}


/*************************************************************************
	Return the length of the next token in String 'text' starting at
	index 'start_idx'.
*************************************************************************/
size_t MultiLineEditbox::getNextTokenLength(const String& text, size_t start_idx) const
{
	String::size_type pos = text.find_first_of(TextUtils::DefaultWrapDelimiters, start_idx);

	// handle case where no more whitespace exists (so this is last token)
	if (pos == String::npos)
	{
		return (text.length() - start_idx);
	}
	// handle 'delimiter' token cases
	else if ((pos - start_idx) == 0)
	{
		return 1;
	}
	else
	{
		return (pos - start_idx);
	}

}


/*************************************************************************
	Return the text code point index that is rendered closest to screen
	position 'pt'.
*************************************************************************/
size_t MultiLineEditbox::getTextIndexFromPosition(const Point& pt) const
{
	//
	// calculate final window position to be checked
	//
	Point wndPt = CoordConverter::screenToWindow(*this, pt);

	Rect textArea(getTextRenderArea());

	wndPt.d_x -= textArea.d_left;
	wndPt.d_y -= textArea.d_top;

	// factor in scroll bar values
	wndPt.d_x += getHorzScrollbar()->getScrollPosition();
	wndPt.d_y += getVertScrollbar()->getScrollPosition();

	size_t lineNumber = static_cast<size_t>(wndPt.d_y / getFont()->getLineSpacing());

	if (lineNumber >= d_lines.size())
	{
		lineNumber = d_lines.size() - 1;
	}

    String lineText(getText().substr(d_lines[lineNumber].d_startIdx, d_lines[lineNumber].d_length));

	size_t lineIdx = getFont()->getCharAtPixel(lineText, wndPt.d_x);

	if (lineIdx >= lineText.length() - 1)
	{
		lineIdx = lineText.length() - 1;
	}

	return d_lines[lineNumber].d_startIdx + lineIdx;
}


/*************************************************************************
	Return the line number a given index falls on with the current
	formatting.  Will return last line if index is out of range.
*************************************************************************/
size_t MultiLineEditbox::getLineNumberFromIndex(size_t index) const
{
	size_t lineCount = d_lines.size();

	if (lineCount == 0)
	{
		return 0;
	}
    else if (index >= getText().length() - 1)
	{
		return lineCount - 1;
	}
	else
	{
		size_t indexCount = 0;
		size_t caratLine = 0;

		for (; caratLine < lineCount; ++caratLine)
		{
			indexCount += d_lines[caratLine].d_length;

			if (index < indexCount)
			{
				return caratLine;
			}

		}

	}

	CEGUI_THROW(InvalidRequestException("MultiLineEditbox::getLineNumberFromIndex - Unable to identify a line from the given, invalid, index."));
}



/*************************************************************************
	Clear the current selection setting
*************************************************************************/
void MultiLineEditbox::clearSelection(void)
{
	// perform action only if required.
	if (getSelectionLength() != 0)
	{
		setSelection(0, 0);
	}

}


/*************************************************************************
	Erase the currently selected text.
*************************************************************************/
void MultiLineEditbox::eraseSelectedText(bool modify_text)
{
	if (getSelectionLength() != 0)
	{
		// setup new carat position and remove selection highlight.
		setCaratIndex(getSelectionStartIndex());

		// erase the selected characters (if required)
		if (modify_text)
		{
            String newText = getText();
            newText.erase(getSelectionStartIndex(), getSelectionLength());
            setText(newText);

			// trigger notification that text has changed.
			WindowEventArgs args(this);
			onTextChanged(args);
		}

		clearSelection();
	}

}


/*************************************************************************
	Processing for backspace key
*************************************************************************/
void MultiLineEditbox::handleBackspace(void)
{
	if (!isReadOnly())
	{
		if (getSelectionLength() != 0)
		{
			eraseSelectedText();
		}
		else if (d_caratPos > 0)
		{
            String newText = getText();
            newText.erase(d_caratPos - 1, 1);
            setCaratIndex(d_caratPos - 1);
            setText(newText);

			WindowEventArgs args(this);
			onTextChanged(args);
		}

	}
}


/*************************************************************************
	Processing for Delete key
*************************************************************************/
void MultiLineEditbox::handleDelete(void)
{
	if (!isReadOnly())
	{
		if (getSelectionLength() != 0)
		{
			eraseSelectedText();
		}
        else if (getCaratIndex() < getText().length() - 1)
		{
            String newText = getText();
            newText.erase(d_caratPos, 1);
            setText(newText);

			ensureCaratIsVisible();

			WindowEventArgs args(this);
			onTextChanged(args);
		}

	}

}


/*************************************************************************
	Processing to move carat one character left
*************************************************************************/
void MultiLineEditbox::handleCharLeft(uint sysKeys)
{
	if (d_caratPos > 0)
	{
		setCaratIndex(d_caratPos - 1);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat one word left
*************************************************************************/
void MultiLineEditbox::handleWordLeft(uint sysKeys)
{
	if (d_caratPos > 0)
	{
        setCaratIndex(TextUtils::getWordStartIdx(getText(), getCaratIndex()));
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat one character right
*************************************************************************/
void MultiLineEditbox::handleCharRight(uint sysKeys)
{
   if (d_caratPos < getText().length() - 1)
	{
		setCaratIndex(d_caratPos + 1);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat one word right
*************************************************************************/
void MultiLineEditbox::handleWordRight(uint sysKeys)
{
   if (d_caratPos < getText().length() - 1)
	{
        setCaratIndex(TextUtils::getNextWordStartIdx(getText(), getCaratIndex()));
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat to the start of the text.
*************************************************************************/
void MultiLineEditbox::handleDocHome(uint sysKeys)
{
	if (d_caratPos > 0)
	{
		setCaratIndex(0);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat to the end of the text
*************************************************************************/
void MultiLineEditbox::handleDocEnd(uint sysKeys)
{
   if (d_caratPos < getText().length() - 1)
	{
       setCaratIndex(getText().length() - 1);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat to the start of the current line.
*************************************************************************/
void MultiLineEditbox::handleLineHome(uint sysKeys)
{
	size_t line = getLineNumberFromIndex(d_caratPos);

	if (line < d_lines.size())
	{
		size_t lineStartIdx = d_lines[line].d_startIdx;

		if (d_caratPos > lineStartIdx)
		{
			setCaratIndex(lineStartIdx);
		}

		if (sysKeys & Shift)
		{
			setSelection(d_caratPos, d_dragAnchorIdx);
		}
		else
		{
			clearSelection();
		}

	}

}


/*************************************************************************
	Processing to move carat to the end of the current line
*************************************************************************/
void MultiLineEditbox::handleLineEnd(uint sysKeys)
{
	size_t line = getLineNumberFromIndex(d_caratPos);

	if (line < d_lines.size())
	{
		size_t lineEndIdx = d_lines[line].d_startIdx + d_lines[line].d_length - 1;

		if (d_caratPos < lineEndIdx)
		{
			setCaratIndex(lineEndIdx);
		}

		if (sysKeys & Shift)
		{
			setSelection(d_caratPos, d_dragAnchorIdx);
		}
		else
		{
			clearSelection();
		}

	}

}


/*************************************************************************
	Processing to move carat up a line.
*************************************************************************/
void MultiLineEditbox::handleLineUp(uint sysKeys)
{
	size_t caratLine = getLineNumberFromIndex(d_caratPos);

	if (caratLine > 0)
	{
        float caratPixelOffset = getFont()->getTextExtent(getText().substr(d_lines[caratLine].d_startIdx, d_caratPos - d_lines[caratLine].d_startIdx));

		--caratLine;

        size_t newLineIndex = getFont()->getCharAtPixel(getText().substr(d_lines[caratLine].d_startIdx, d_lines[caratLine].d_length), caratPixelOffset);

		setCaratIndex(d_lines[caratLine].d_startIdx + newLineIndex);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to move carat down a line.
*************************************************************************/
void MultiLineEditbox::handleLineDown(uint sysKeys)
{
	size_t caratLine = getLineNumberFromIndex(d_caratPos);

	if ((d_lines.size() > 1) && (caratLine < (d_lines.size() - 1)))
	{
        float caratPixelOffset = getFont()->getTextExtent(getText().substr(d_lines[caratLine].d_startIdx, d_caratPos - d_lines[caratLine].d_startIdx));

		++caratLine;

        size_t newLineIndex = getFont()->getCharAtPixel(getText().substr(d_lines[caratLine].d_startIdx, d_lines[caratLine].d_length), caratPixelOffset);

		setCaratIndex(d_lines[caratLine].d_startIdx + newLineIndex);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Processing to insert a new line / paragraph.
*************************************************************************/
void MultiLineEditbox::handleNewLine(uint /*sysKeys*/)
{
	if (!isReadOnly())
	{
		// erase selected text
		eraseSelectedText();

		// if there is room
       if (getText().length() - 1 < d_maxTextLen)
		{
            String newText = getText();
            newText.insert(getCaratIndex(), 1, 0x0a);
            setText(newText);

			d_caratPos++;

			WindowEventArgs args(this);
			onTextChanged(args);
		}

	}

}


/*************************************************************************
    Processing to move caret one page up
*************************************************************************/
void MultiLineEditbox::handlePageUp(uint sysKeys)
{
    size_t caratLine = getLineNumberFromIndex(d_caratPos);
    size_t nbLine = static_cast<size_t>(getTextRenderArea().getHeight() / getFont()->getLineSpacing());
    size_t newline = 0;
    if (nbLine < caratLine)
    {
        newline = caratLine - nbLine;
    }
    setCaratIndex(d_lines[newline].d_startIdx);
    if (sysKeys & Shift)
    {
        setSelection(d_caratPos, d_selectionEnd);
    }
    else
    {
        clearSelection();
    }
    ensureCaratIsVisible();
}


/*************************************************************************
    Processing to move caret one page down
*************************************************************************/
void MultiLineEditbox::handlePageDown(uint sysKeys)
{
    size_t caratLine = getLineNumberFromIndex(d_caratPos);
    size_t nbLine =  static_cast<size_t>(getTextRenderArea().getHeight() / getFont()->getLineSpacing());
    size_t newline = caratLine + nbLine;
    if (d_lines.size() > 0)
    {
        newline = newline < d_lines.size() - 1 ? newline : d_lines.size() -1;
    }
    setCaratIndex(d_lines[newline].d_startIdx + d_lines[newline].d_length - 1);
    if (sysKeys & Shift)
    {
        setSelection(d_selectionStart, d_caratPos);
    }
    else
    {
        clearSelection();
    }
    ensureCaratIsVisible();
}


/*************************************************************************
	Handler for when a mouse button is pushed
*************************************************************************/
void MultiLineEditbox::onMouseButtonDown(MouseEventArgs& e)
{
	// base class handling
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		// grab inputs
		if (captureInput())
		{
			// handle mouse down
			clearSelection();
			d_dragging = true;
			d_dragAnchorIdx = getTextIndexFromPosition(e.position);
			setCaratIndex(d_dragAnchorIdx);
		}

		++e.handled;
	}

}


/*************************************************************************
	Handler for when mouse button is released
*************************************************************************/
void MultiLineEditbox::onMouseButtonUp(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();
		++e.handled;
	}

}


/*************************************************************************
	Handler for when mouse button is double-clicked
*************************************************************************/
void MultiLineEditbox::onMouseDoubleClicked(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseDoubleClicked(e);

	if (e.button == LeftButton)
	{
        d_dragAnchorIdx = TextUtils::getWordStartIdx(getText(), (d_caratPos == getText().length()) ? d_caratPos : d_caratPos + 1);
        d_caratPos      = TextUtils::getNextWordStartIdx(getText(), d_caratPos);

		// perform actual selection operation.
		setSelection(d_dragAnchorIdx, d_caratPos);

		++e.handled;
	}

}


/*************************************************************************
	Handler for when mouse button is triple-clicked.
*************************************************************************/
void MultiLineEditbox::onMouseTripleClicked(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseTripleClicked(e);

	if (e.button == LeftButton)
	{
		size_t caratLine = getLineNumberFromIndex(d_caratPos);
		size_t lineStart = d_lines[caratLine].d_startIdx;

		// find end of last paragraph
        String::size_type paraStart = getText().find_last_of(d_lineBreakChars, lineStart);

		// if no previous paragraph, selection will start at the beginning.
		if (paraStart == String::npos)
		{
			paraStart = 0;
		}

		// find end of this paragraph
        String::size_type paraEnd = getText().find_first_of(d_lineBreakChars, lineStart);

		// if paragraph has no end, which actually should never happen, fix the
		// erroneous situation and select up to end at end of text.
		if (paraEnd == String::npos)
		{
            String newText = getText();
            newText.append(1, '\n');
            setText(newText);

            paraEnd = getText().length() - 1;
		}

		// set up selection using new values.
		d_dragAnchorIdx = paraStart;
		setCaratIndex(paraEnd);
		setSelection(d_dragAnchorIdx, d_caratPos);
		++e.handled;
	}

}


/*************************************************************************
	Handler for when mouse moves in the window.
*************************************************************************/
void MultiLineEditbox::onMouseMove(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseMove(e);

	if (d_dragging)
	{
		setCaratIndex(getTextIndexFromPosition(e.position));
		setSelection(d_caratPos, d_dragAnchorIdx);
	}

	++e.handled;
}


/*************************************************************************
	Handler for when capture is lost.
*************************************************************************/
void MultiLineEditbox::onCaptureLost(WindowEventArgs& e)
{
	d_dragging = false;

	// base class processing
	Window::onCaptureLost(e);

	++e.handled;
}


/*************************************************************************
	Handler for when character (printable keys) are typed
*************************************************************************/
void MultiLineEditbox::onCharacter(KeyEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propogates
    // inputs back up the window hierarchy, whereas, as a consumer of key
    // events, we want such propogation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventCharacterKey, e, Window::EventNamespace);

	// only need to take notice if we have focus
	if (e.handled == 0 && hasInputFocus() && !isReadOnly() &&
        getFont()->isCodepointAvailable(e.codepoint))
	{
		// erase selected text
		eraseSelectedText();

		// if there is room
       if (getText().length() - 1 < d_maxTextLen)
		{
           String newText = getText();
           newText.insert(getCaratIndex(), 1, e.codepoint);
           setText(newText);

			d_caratPos++;

			WindowEventArgs args(this);
			onTextChanged(args);

            ++e.handled;
		}
		else
		{
			// Trigger text box full event
			WindowEventArgs args(this);
			onEditboxFullEvent(args);
		}

	}

}


/*************************************************************************
	Handler for when non-printable keys are typed.
*************************************************************************/
void MultiLineEditbox::onKeyDown(KeyEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propogates
    // inputs back up the window hierarchy, whereas, as a consumer of key
    // events, we want such propogation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventKeyDown, e, Window::EventNamespace);

	if (e.handled == 0 && hasInputFocus() && !isReadOnly())
	{
		WindowEventArgs args(this);
		switch (e.scancode)
		{
		case Key::LeftShift:
		case Key::RightShift:
			if (getSelectionLength() == 0)
			{
				d_dragAnchorIdx = getCaratIndex();
			}
			break;

		case Key::Backspace:
			handleBackspace();
			break;

		case Key::Delete:
			handleDelete();
			break;

		case Key::Return:
		case Key::NumpadEnter:
			handleNewLine(e.sysKeys);
			break;

		case Key::ArrowLeft:
			if (e.sysKeys & Control)
			{
				handleWordLeft(e.sysKeys);
			}
			else
			{
				handleCharLeft(e.sysKeys);
			}
			break;

		case Key::ArrowRight:
			if (e.sysKeys & Control)
			{
				handleWordRight(e.sysKeys);
			}
			else
			{
				handleCharRight(e.sysKeys);
			}
			break;

		case Key::ArrowUp:
			handleLineUp(e.sysKeys);
			break;

		case Key::ArrowDown:
			handleLineDown(e.sysKeys);
			break;

		case Key::Home:
			if (e.sysKeys & Control)
			{
				handleDocHome(e.sysKeys);
			}
			else
			{
				handleLineHome(e.sysKeys);
			}
			break;

		case Key::End:
			if (e.sysKeys & Control)
			{
				handleDocEnd(e.sysKeys);
			}
			else
			{
				handleLineEnd(e.sysKeys);
			}
			break;

        case Key::PageUp:
            handlePageUp(e.sysKeys);
            break;

        case Key::PageDown:
            handlePageDown(e.sysKeys);
            break;

        // default case is now to leave event as (possibly) unhandled.
        default:
            return;
		}

		++e.handled;
	}

}


/*************************************************************************
	Handler for when text is programmatically changed.
*************************************************************************/
void MultiLineEditbox::onTextChanged(WindowEventArgs& e)
{
    // ensure last character is a new line
    if ((getText().length() == 0) || (getText()[getText().length() - 1] != '\n'))
    {
        String newText = getText();
        newText.append(1, '\n');
        setText(newText);
    }


    // base class processing
    Window::onTextChanged(e);

    // clear selection
    clearSelection();
    // layout new text
    formatText(true);
    // layout child windows (scrollbars) since text layout may have changed
    performChildWindowLayout();
    // ensure carat is still within the text
    setCaratIndex(getCaratIndex());
    // ensure carat is visible
    // NB: this will already have been called at least once, but since we
    // may have changed the formatting of the text, it needs to be called again.
    ensureCaratIsVisible();

    ++e.handled;
}


/*************************************************************************
	Handler for when widget size is changed.
*************************************************************************/
void MultiLineEditbox::onSized(WindowEventArgs& e)
{
	formatText(true);

	// base class handling
	Window::onSized(e);

	++e.handled;
}


/*************************************************************************
	Handler for mouse wheel changes
*************************************************************************/
void MultiLineEditbox::onMouseWheel(MouseEventArgs& e)
{
	// base class processing.
	Window::onMouseWheel(e);

    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

	if (vertScrollbar->isVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
	{
		vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.wheelChange);
	}
	else if (horzScrollbar->isVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
	{
		horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.wheelChange);
	}

	++e.handled;
}


/*************************************************************************
	Handler called when the read-only state of the edit box changes
*************************************************************************/
void MultiLineEditbox::onReadOnlyChanged(WindowEventArgs& e)
{
	fireEvent(EventReadOnlyModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the word wrap mode for the the edit box changes
*************************************************************************/
void MultiLineEditbox::onWordWrapModeChanged(WindowEventArgs& e)
{
	fireEvent(EventWordWrapModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the maximum text length for the edit box changes
*************************************************************************/
void MultiLineEditbox::onMaximumTextLengthChanged(WindowEventArgs& e)
{
	fireEvent(EventMaximumTextLengthChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the carat moves.
*************************************************************************/
void MultiLineEditbox::onCaratMoved(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventCaratMoved, e, EventNamespace);
}


/*************************************************************************
	Handler called when the text selection changes
*************************************************************************/
void MultiLineEditbox::onTextSelectionChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventTextSelectionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the edit box is full
*************************************************************************/
void MultiLineEditbox::onEditboxFullEvent(WindowEventArgs& e)
{
	fireEvent(EventEditboxFull, e, EventNamespace);
}


/*************************************************************************
	Handler called when the 'always show' setting for the vertical
	scroll bar changes.
*************************************************************************/
void MultiLineEditbox::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when 'always show' setting for the horizontal scroll
	bar changes.
*************************************************************************/
void MultiLineEditbox::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}


/*************************************************************************
	Return whether the text in the edit box will be word-wrapped.
*************************************************************************/
bool MultiLineEditbox::isWordWrapped(void) const
{
	return d_wordWrap;
}


/*************************************************************************
	Add new properties for this class
*************************************************************************/
void MultiLineEditbox::addMultiLineEditboxProperties(void)
{
	addProperty(&d_readOnlyProperty);
	addProperty(&d_wordWrapProperty);
	addProperty(&d_caratIndexProperty);
	addProperty(&d_selectionStartProperty);
	addProperty(&d_selectionLengthProperty);
	addProperty(&d_maxTextLengthProperty);
    addProperty(&d_selectionBrushProperty);
    addProperty(&d_forceVertProperty);
}

/*************************************************************************
    Handler for scroll position changes.
*************************************************************************/
bool MultiLineEditbox::handle_scrollChange(const EventArgs&)
{
    // simply trigger a redraw of the Listbox.
    invalidate();
    return true;
}

/*************************************************************************
    Return a pointer to the vertical scrollbar component widget.
*************************************************************************/
Scrollbar* MultiLineEditbox::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(
                                   getName() + VertScrollbarNameSuffix));
}

/*************************************************************************
	Return whether the vertical scroll bar is always shown.
*************************************************************************/
bool MultiLineEditbox::isVertScrollbarAlwaysShown(void) const
{
	return d_forceVertScroll;
}

/*************************************************************************
    Return a pointer to the horizontal scrollbar component widget.
*************************************************************************/
Scrollbar* MultiLineEditbox::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(
                                   getName() + HorzScrollbarNameSuffix));
}

/*************************************************************************
    Get the text rendering area
*************************************************************************/
Rect MultiLineEditbox::getTextRenderArea() const
{
    if (d_windowRenderer != 0)
    {
        MultiLineEditboxWindowRenderer* wr = (MultiLineEditboxWindowRenderer*)d_windowRenderer;
        return wr->getTextRenderArea();
    }
    else
    {
        //return getTextRenderArea_impl();
        CEGUI_THROW(InvalidRequestException("MultiLineEditbox::getTextRenderArea - This function must be implemented by the window renderer module"));
    }
}

const Image* MultiLineEditbox::getSelectionBrushImage() const
{
    return d_selectionBrush;
}

void MultiLineEditbox::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidate();
}

/*************************************************************************
	Set whether the vertical scroll bar should always be shown.
*************************************************************************/
void MultiLineEditbox::setShowVertScrollbar(bool setting)
{
	if (d_forceVertScroll != setting)
	{
		d_forceVertScroll = setting;

		configureScrollbars();
		WindowEventArgs args(this);
		onVertScrollbarModeChanged(args);
	}
}

//----------------------------------------------------------------------------//
bool MultiLineEditbox::handle_vertScrollbarVisibilityChanged(const EventArgs&)
{
    if (d_wordWrap)
        formatText(false);

    return true;
}


} // End of  CEGUI namespace section
