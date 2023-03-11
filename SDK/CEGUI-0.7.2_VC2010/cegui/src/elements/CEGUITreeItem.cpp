/***********************************************************************
     filename:   CEGUITreeItem.cpp
     created:	5-13-07
     author:		Jonathan Welch (Based on Code by David Durant)
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "elements/CEGUITree.h"
#include "elements/CEGUITreeItem.h"
#include "CEGUISystem.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindow.h"
#include "CEGUIImage.h"
#include <algorithm>

#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUIFribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUIMinibidiVisualMapping.h"
#else
    #include "CEGUIBiDiVisualMapping.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
BasicRenderedStringParser TreeItem::d_stringParser;

/*************************************************************************
    Constants
*************************************************************************/
const colour TreeItem::DefaultSelectionColour = 0xFF4444AA;
const colour TreeItem::DefaultTextColour = 0xFFFFFFFF;

/*************************************************************************
    Base class constructor
*************************************************************************/
TreeItem::TreeItem(const String& text, uint item_id, void* item_data,
                   bool disabled, bool auto_delete) :
#ifndef CEGUI_BIDI_SUPPORT
    d_bidiVisualMapping(0),
#elif defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(new MinibidiVisualMapping),
#else
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
    d_bidiDataValid(false),
    d_itemID(item_id),
    d_itemData(item_data),
    d_selected(false),
    d_disabled(disabled),
    d_autoDelete(auto_delete),
    d_buttonLocation(Rect(0,0,0,0)),
    d_owner(0),
    d_selectCols(DefaultSelectionColour, DefaultSelectionColour,
                 DefaultSelectionColour, DefaultSelectionColour),
    d_selectBrush(0),
    d_textCols(DefaultTextColour, DefaultTextColour,
               DefaultTextColour, DefaultTextColour),
    d_font(0),
    d_iconImage(0),
    d_isOpen(false),
    d_renderedStringValid(false)
{
    setText(text);
}

//----------------------------------------------------------------------------//
TreeItem::~TreeItem(void)
{
    delete d_bidiVisualMapping;
}

/*************************************************************************
    Set the selection highlighting brush image.
*************************************************************************/
void TreeItem::setSelectionBrushImage(const String& imageset,
                                      const String& image)
{
    setSelectionBrushImage(
        &ImagesetManager::getSingleton().get(imageset).getImage(image));
}

/*************************************************************************
    Return a ColourRect object describing the colours in 'cols' after
    having their alpha component modulated by the value 'alpha'.
*************************************************************************/
ColourRect TreeItem::getModulateAlphaColourRect(const ColourRect& cols,
                                                float alpha) const
{
    return ColourRect
    (
     calculateModulatedAlphaColour(cols.d_top_left, alpha),
     calculateModulatedAlphaColour(cols.d_top_right, alpha),
     calculateModulatedAlphaColour(cols.d_bottom_left, alpha),
     calculateModulatedAlphaColour(cols.d_bottom_right, alpha)
     );
}

/*************************************************************************
    Return a colour value describing the colour specified by 'col' after
    having its alpha component modulated by the value 'alpha'.
*************************************************************************/
colour TreeItem::calculateModulatedAlphaColour(colour col, float alpha) const
{
    colour temp(col);
    temp.setAlpha(temp.getAlpha() * alpha);
    return temp;
}

/*************************************************************************
    Return a pointer to the font being used by this TreeItem
*************************************************************************/
Font* TreeItem::getFont(void) const
{
    // prefer out own font
    if (d_font != 0)
        return d_font;
    // try our owner window's font setting
    // (may be null if owner uses non existant default font)
    else if (d_owner != 0)
        return d_owner->getFont();
    // no owner, just use the default (which may be NULL anyway)
    else
        return System::getSingleton().getDefaultFont();   
}

/*************************************************************************
    Set the font to be used by this TreeItem
*************************************************************************/
void TreeItem::setFont(const String& font_name)
{
    setFont(&FontManager::getSingleton().get(font_name));
}

//----------------------------------------------------------------------------//
void TreeItem::setFont(Font* font)
{
    d_font = font;

    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//

/*************************************************************************
    Return the rendered pixel size of this tree item.
*************************************************************************/
Size TreeItem::getPixelSize(void) const
{
    Font* fnt = getFont();

    if (!fnt)
        return Size(0, 0);

    if (!d_renderedStringValid)
        parseTextString();

    Size sz(0.0f, 0.0f);

    for (size_t i = 0; i < d_renderedString.getLineCount(); ++i)
    {
        const Size line_sz(d_renderedString.getPixelSize(i));
        sz.d_height += line_sz.d_height;

        if (line_sz.d_width > sz.d_width)
            sz.d_width = line_sz.d_width;
    }

    return sz;
}

/*************************************************************************
    Add the given TreeItem to this item's list.
*************************************************************************/
void TreeItem::addItem(TreeItem* item)
{
    if (item != 0)
    {
        Tree* parentWindow = (Tree*)getOwnerWindow();
        
        // establish ownership
        item->setOwnerWindow(parentWindow);
        
        // if sorting is enabled, re-sort the tree
        if (parentWindow->isSortEnabled())
        {
            d_listItems.insert(
                std::upper_bound(d_listItems.begin(),
                                 d_listItems.end(), item, &lbi_less),
                item);
        }
        // not sorted, just stick it on the end.
        else
        {
            d_listItems.push_back(item);
        }
        
        WindowEventArgs args(parentWindow);
        parentWindow->onListContentsChanged(args);
    }
}

/*************************************************************************
    Remove the given TreeItem from this item's list.
*************************************************************************/
void TreeItem::removeItem(const TreeItem* item) 
{ 
    if (item)
    {
        Tree* parentWindow = (Tree*)getOwnerWindow();

        LBItemList::iterator pos = std::find(d_listItems.begin(),
                                             d_listItems.end(),
                                             item);
        if (pos != d_listItems.end())
        {
            (*pos)->setOwnerWindow(0);
            d_listItems.erase(pos);

            if (item == parentWindow->d_lastSelected)
                parentWindow->d_lastSelected = 0;

            if (item->isAutoDeleted())
                delete item;

            WindowEventArgs args(parentWindow);
            parentWindow->onListContentsChanged(args);
        }
    }
}

TreeItem *TreeItem::getTreeItemFromIndex(size_t itemIndex)
{
    if (itemIndex > d_listItems.size())
        return 0;
    
    return d_listItems[itemIndex];
}

/*************************************************************************
    Draw the tree item in its current state.
*************************************************************************/
void TreeItem::draw(GeometryBuffer& buffer, const Rect &targetRect,
                    float alpha, const Rect *clipper) const
{
    Rect finalRect(targetRect);

    if (d_iconImage != 0)
    {
        Rect finalPos(finalRect);
        finalPos.setWidth(targetRect.getHeight());
        finalPos.setHeight(targetRect.getHeight());
        d_iconImage->draw(buffer, finalPos, clipper,
                          ColourRect(colour(1,1,1,alpha)));
        finalRect.d_left += targetRect.getHeight();
    }

    if (d_selected && d_selectBrush != 0)
        d_selectBrush->draw(buffer, finalRect, clipper,
                            getModulateAlphaColourRect(d_selectCols, alpha));

    Font* font = getFont();

    if (!font)
        return;

    Vector2 draw_pos(finalRect.getPosition());
    draw_pos.d_y -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;

    if (!d_renderedStringValid)
        parseTextString();

    const ColourRect final_colours(
        getModulateAlphaColourRect(ColourRect(0xFFFFFFFF), alpha));

    for (size_t i = 0; i < d_renderedString.getLineCount(); ++i)
    {
        d_renderedString.draw(i, buffer, draw_pos, &final_colours, clipper, 0.0f);
        draw_pos.d_y += d_renderedString.getPixelSize(i).d_height;
    }
}

/*************************************************************************
    Set the colours used for selection highlighting.
*************************************************************************/
void TreeItem::setSelectionColours(colour top_left_colour,
                                   colour top_right_colour,
                                   colour bottom_left_colour,
                                   colour bottom_right_colour)
{
    d_selectCols.d_top_left		= top_left_colour;
    d_selectCols.d_top_right	= top_right_colour;
    d_selectCols.d_bottom_left	= bottom_left_colour;
    d_selectCols.d_bottom_right	= bottom_right_colour;
}

/*************************************************************************
    Set the colours used for text rendering.
*************************************************************************/
void TreeItem::setTextColours(colour top_left_colour,
                              colour top_right_colour,
                              colour bottom_left_colour,
                              colour bottom_right_colour)
{
    d_textCols.d_top_left		= top_left_colour;
    d_textCols.d_top_right		= top_right_colour;
    d_textCols.d_bottom_left	= bottom_left_colour;
    d_textCols.d_bottom_right	= bottom_right_colour;

    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
void TreeItem::setText( const String& text )
{
    d_textLogical = text;
    d_bidiDataValid = false;
    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
void TreeItem::parseTextString() const
{
    d_renderedString =
        d_stringParser.parse(getTextVisual(), getFont(), &d_textCols);
    d_renderedStringValid = true;
}

//----------------------------------------------------------------------------//
const String& TreeItem::getTextVisual() const
{
    // no bidi support
    if (!d_bidiVisualMapping)
        return d_textLogical;

    if (!d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(d_textLogical);
        d_bidiDataValid = true;
    }

    return d_bidiVisualMapping->getTextVisual();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
