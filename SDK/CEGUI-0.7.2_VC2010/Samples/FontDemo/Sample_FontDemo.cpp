/***********************************************************************
    filename:   FontDemo.cpp
    created:    17/6/2006
    author:     Paul D Turner
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

// NOTE: if you don't have Asian fonts installed, the Japanese and Korean
// sample texts might show up unreadable in this source file. The sample
// should look okay though when running.

#include "CEGuiSample.h"
#include "CEGUI.h"
#include "CEGUIPropertyHelper.h"

using namespace CEGUI;

static struct
{
    utf8 *Language;
    utf8* Font;
	utf8 *Text;
} LangList [] =
{
	// A list of strings in different languages
	// Feel free to add your own language here (UTF-8 ONLY!)...
    { (utf8 *)"English",
      (utf8*)"DejaVuSans-10",
	  (utf8 *)"THIS IS SOME TEXT IN UPPERCASE\n"
              "and this is lowercase...\n"
              "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog" },
    { (utf8 *)"Русский",
      (utf8*)"DejaVuSans-10",
	  (utf8 *)"Всё ускоряющаяся эволюция компьютерных технологий предъявила жёсткие требования к производителям как собственно вычислительной техники, так и периферийных устройств.\n"
              "\nЗавершён ежегодный съезд эрудированных школьников, мечтающих глубоко проникнуть в тайны физических явлений и химических реакций.\n"
              "\nавтор панграмм -- Андрей Николаев\n" },
    { (utf8 *)"Română",
      (utf8*)"DejaVuSans-10",
      (utf8 *)"CEI PATRU APOSTOLI\n"
              "au fost trei:\n"
              "Luca şi Matfei\n" },
    { (utf8 *)"Dansk",
      (utf8*)"DejaVuSans-10",
      (utf8 *)"FARLIGE STORE BOGSTAVER\n"
              "og flere men små...\n"
              "Quizdeltagerne spiste jordbær med fløde, mens cirkusklovnen Walther spillede på xylofon\n" },
	{ (utf8 *)"Japanese",
      (utf8*)"Sword-26",
      (utf8 *)"日本語を選択\n"
              "トリガー検知\n"
              "鉱石備蓄不足\n" },
	{ (utf8 *)"Korean",
      (utf8*)"Batang-26",
      (utf8 *)"한국어를 선택\n"
              "트리거 검지\n"
              "광석 비축부족\n" },
    { (utf8 *)"Việt",
      (utf8*)"DejaVuSans-10",
      (utf8 *)"Chào CrazyEddie !\n"
              "Mình rất hạnh phúc khi nghe bạn nói điều đó\n"
              "Hy vọng sớm được thấy CEGUI hỗ trợ đầy đủ tiếng Việt\n"
              "Cám ơn bạn rất nhiều\n"
              "Chúc bạn sức khoẻ\n"
              "Tạm biệt !\n" }
};

#define MIN_POINT_SIZE 6.0f

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public ListboxTextItem
{
public:
    MyListItem (const String& text, CEGUI::uint item_id = 0) :
        ListboxTextItem(text, item_id)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};

// Sample class
class FontDemo : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample ()
	{
        // we will use of the WindowManager.
        WindowManager& winMgr = WindowManager::getSingleton ();

        // load scheme and set up defaults
        SchemeManager::getSingleton().create("TaharezLook.scheme");
        System::getSingleton().setDefaultMouseCursor ("TaharezLook", "MouseArrow");

		// Create a custom font which we use to draw the list items. This custom
		// font won't get effected by the scaler and such.
		FontManager::getSingleton().createFreeTypeFont("DefaultFont", 10/*pt*/, true, "DejaVuSans.ttf");
		// Set it as the default
		System::getSingleton().setDefaultFont("DefaultFont");

        // load all the fonts (if they are not loaded yet)
        FontManager::getSingleton().createAll("*.font", "fonts");

        // load an image to use as a background
        ImagesetManager::getSingleton().createFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

        // here we will use a StaticImage as the root, then we can use it to place a background image
        Window* background = winMgr.createWindow ("TaharezLook/StaticImage");
        // set area rectangle
        background->setArea (URect (cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)));
        // disable frame and standard background
        background->setProperty ("FrameEnabled", "false");
        background->setProperty ("BackgroundEnabled", "false");
        // set the background image
        background->setProperty ("Image", "set:BackgroundImage image:full_image");
        // install this as the root GUI sheet
        System::getSingleton ().setGUISheet (background);

        // set tooltip styles (by default there is none)
        System::getSingleton ().setDefaultTooltip ("TaharezLook/Tooltip");

        // load some demo windows and attach to the background 'root'
        background->addChildWindow (winMgr.loadWindowLayout ("FontDemo.layout"));

        // Add the font names to the listbox
        Listbox *lbox = static_cast<Listbox *> (winMgr.getWindow ("FontDemo/FontList"));
		lbox->setFont("DefaultFont");

        FontManager::FontIterator fi = FontManager::getSingleton().getIterator();    
        while (!fi.isAtEnd())
        {
            // exclude the special DefaultFont!
            if (fi.getCurrentKey() != String("DefaultFont"))
                lbox->addItem(new MyListItem(fi.getCurrentKey()));
            ++fi;
        }

        // set up the font listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleFontSelection, this));
        // select the first font
        lbox->setItemSelectState (size_t (0), true);

        // Add language list to the listbox
        lbox = static_cast<Listbox *> (winMgr.getWindow ("FontDemo/LangList"));
		lbox->setFont("DefaultFont");
        for (size_t i = 0; i < (sizeof (LangList) / sizeof (LangList [0])); i++)
            // only add a language if 'preferred' font is available
            if (FontManager::getSingleton().isDefined(String(LangList[i].Font)))
                lbox->addItem (new MyListItem (LangList [i].Language, i));
        // set up the language listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleLangSelection, this));
        // select the first language
        lbox->setItemSelectState (size_t (0), true);

        winMgr.getWindow("FontDemo/AutoScaled")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAutoScaled, this));
        winMgr.getWindow("FontDemo/Antialiased")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAntialiased, this));
        winMgr.getWindow("FontDemo/PointSize")->subscribeEvent (
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber (&FontDemo::handlePointSize, this));

        return true;
	}

    // method to perform any required cleanup operations.
    void cleanupSample ()
    {
        // me? cleanup? what?
    }

	/** When a fonts get selected from the list, we update the name field. Of course,
	this can be done easier (by passing the selected font), but this demonstrates how 
	to query a widget's font. */
    void setFontDesc ()
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

		// Query the font from the textbox
        Font *f = mle->getFont ();

		// Build up the font name...
        String s = f->getProperty ("Name");
        if (f->isPropertyPresent ("PointSize"))
            s += "." + f->getProperty ("PointSize");

		// ...and set it
        winMgr.getWindow("FontDemo/FontDesc")->setText (s);
    }

	/** Called when the used selects a different font from the font list.*/
    bool handleFontSelection (const EventArgs& e)
    {
		// Access the listbox which sent the event
        Listbox *lbox = static_cast<Listbox*> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
		{	// Read the fontname and get the font by that name
            Font *font = &FontManager::getSingleton ().get(
                lbox->getFirstSelectedItem ()->getText ());

			// Tell the textbox to use the newly selected font
            WindowManager& winMgr = WindowManager::getSingleton ();
            winMgr.getWindow("FontDemo/FontSample")->setFont (font);

            bool b = font->isPropertyPresent ("AutoScaled");
            Checkbox *cb = static_cast<Checkbox *> (winMgr.getWindow("FontDemo/AutoScaled"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper::stringToBool (font->getProperty ("AutoScaled")));

            b = font->isPropertyPresent ("Antialiased");
            cb = static_cast<Checkbox *> (winMgr.getWindow("FontDemo/Antialiased"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper::stringToBool (font->getProperty ("Antialiased")));

            b = font->isPropertyPresent ("PointSize");
            Scrollbar *sb = static_cast<Scrollbar *> (
                winMgr.getWindow("FontDemo/PointSize"));
            sb->setEnabled (b);
			
			// Set the textbox' font to have the current scale
			if (font->isPropertyPresent("PointSize"))
				font->setProperty ("PointSize",
                        PropertyHelper::intToString (
                            int (MIN_POINT_SIZE + sb->getScrollPosition ())));

            setFontDesc ();
		}

        return true;
    }

    bool handleAutoScaled (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

        Font *f = mle->getFont ();
        f->setProperty ("AutoScaled",
                        PropertyHelper::boolToString (cb->isSelected ()));

        updateTextWindows();
        return true;
    }

    bool handleAntialiased (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

        Font *f = mle->getFont ();
        f->setProperty ("Antialiased",
                        PropertyHelper::boolToString (cb->isSelected ()));

        updateTextWindows();
        return true;
    }

    bool handlePointSize (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Scrollbar *sb = static_cast<Scrollbar *> (
            static_cast<const WindowEventArgs&> (e).window);

        Font *f = winMgr.getWindow ("FontDemo/FontSample")->getFont ();

        f->setProperty ("PointSize",
                        PropertyHelper::intToString (
                            int (MIN_POINT_SIZE + sb->getScrollPosition ())));

        setFontDesc ();

        updateTextWindows();
        return true;
    }

	/** User selects a new language. Change the textbox content, and start with
	the recommended font. */
    bool handleLangSelection (const EventArgs& e)
    {
		// Access the listbox which sent the event
        Listbox *lbox = static_cast<Listbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
        {
            ListboxItem* sel_item = lbox->getFirstSelectedItem();
            size_t idx = sel_item ? sel_item->getID() : 0;
            const String fontName(LangList[idx].Font);

            WindowManager& winMgr = WindowManager::getSingleton ();
			// Access the font list
			Listbox *fontList = static_cast<Listbox*>(winMgr.getWindow ("FontDemo/FontList"));
            ListboxItem* lbi = fontList->findItemWithText(fontName, 0);
			// Select correct font when not set already
			if (lbi && !lbi->isSelected())
			{	// This will cause 'handleFontSelection' to get called(!)
				fontList->setItemSelectState(lbi, true);
			}

			// Finally, set the sample text for the selected language
            winMgr.getWindow ("FontDemo/FontSample")->setText ((utf8*)LangList [idx].Text);
        }

        return true;
    }

    //! Ensure window content and layout is updated.
    void updateTextWindows()
    {
        WindowManager& winMgr(WindowManager::getSingleton());
        MultiLineEditbox* eb = static_cast<MultiLineEditbox*>(
            winMgr.getWindow("FontDemo/FontSample"));
        // this is a hack to force the editbox to update it's state, and is
        // needed because no facility currently exists for a font to notify that
        // it's internal size or state has changed (ideally all affected windows
        // should receive EventFontChanged - this should be a TODO item!)
		eb->setWordWrapping(false);
		eb->setWordWrapping(true);
        // inform lists of updated data too
        Listbox* lb = static_cast<Listbox*>(winMgr.getWindow("FontDemo/LangList"));
		lb->handleUpdatedItemData();
        lb = static_cast<Listbox*>(winMgr.getWindow("FontDemo/FontList"));
		lb->handleUpdatedItemData();
    }
};

int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    FontDemo app;
    return app.run ();
}
