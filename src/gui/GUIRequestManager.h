#ifndef GUIREQUESTMANAGER_H
#define GUIREQUESTMANAGER_H

#include <irrlicht.h>
#include <vector>

#include "CGUIFileSelector.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


class GUIRequestManager
{
    public:
		GUIRequestManager();
		~GUIRequestManager();
        static GUIRequestManager* getInstance();
		void FileSelector();
		void update();
		inline bool isComplete() {return iscomplete;}
		inline stringw getFilename() {return filename;}

	private:
		CGUIFileSelector* selector;
		irr::IrrlichtDevice* filedevice;
		irr::gui::IGUIEnvironment* guienv;
		bool iscomplete;
		irr::core::stringw filename;


};

#endif // GUIREQUESTMANAGER_H