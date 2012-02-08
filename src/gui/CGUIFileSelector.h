#ifndef INC_CGUIFILESELECTOR_H
#define INC_CGUIFILESELECTOR_H

#include <irrlicht.h>
#ifdef WIN32
   #include <windows.h>
	#include <Shlobj.h>
   #include <iostream>
#endif

using namespace irr;
using namespace gui;

/** Class for opening/saving files. */
//class CGUIFileSelector : public IGUIFileOpenDialog {
class CGUIFileSelector : public IGUIElement {
      
   public:
        /** Enum to specify the usage of the instance of the class */   
        enum E_FILESELECTOR_TYPE {
          EFST_OPEN_DIALOG, //<! For opening files
          EFST_SAVE_DIALOG, //<! For saving files
          EFST_NUM_TYPES    //<! Not used, just specifies how many possible types there are
        };
                  
        /** 
      \brief Constructor
      \param title - The title of the dialog
      \pararm environment - The GUI environment to be used
      \param parent - The parent of the dialog
      \param id - The ID of the dialog
      \param type - The type of dialog
      */
      CGUIFileSelector(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id, E_FILESELECTOR_TYPE type);

      /**
        \brief Destructor
        */
      virtual ~CGUIFileSelector();
      
      /**
        \brief Returns the filename of the selected file. Returns NULL, if no file was selected.
        \return a const wchar_t*
        */
      virtual const wchar_t* getFileName() const;

      /**
        \brief called if an event happened.
        \param even - the event that happened
        \return a bool
        */
      virtual bool OnEvent(const SEvent& event);

      /**
      \brief Render function
      */
      virtual void draw();

	  //! Returns the directory of the selected file. Returns NULL, if no directory was selected.
	  virtual const io::path& getDirectoryName();

      /**
      \brief Returns the current file filter selected or "All Files" if no filter is applied
      \return a stringw
      */
      inline core::stringw getFileFilter() const {
               if (FilterComboBox->getSelected() >= (s32)FileFilters.size()) return core::stringw("All Files"); 
               else return FileFilters[FilterComboBox->getSelected()].FileExtension;
        }
        
        /**
        \brief Returns the type of the dialog
        \return a E_FILESELECTOR_TYPE
        */
        inline E_FILESELECTOR_TYPE getDialogType() { return DialogType; }
        
        /**
        \brief Add a file filter
        \param name - The description of the file type
        \param ext - The file's extension
        \param texture - The icon texture
        */
        void addFileFilter(wchar_t* name, wchar_t* ext, video::ITexture* texture);
        
        /**
        \brief Set an icon to use to display unknown files
        \param texture - the 16x16 icon to use
        */
        inline void setCustomFileIcon(video::ITexture* texture) {
          if (texture) FileIconIdx = addIcon(texture);
          else FileIconIdx = -1;
          fillListBox();
        }
        /**
        \brief Set an icon to use to display directories
        \param texture - the 16x16 icon to use
        */
        inline void setCustomDirectoryIcon(video::ITexture* texture) {
          if (texture) DirectoryIconIdx = addIcon(texture);
          else DirectoryIconIdx = -1;
          fillListBox();
        }

		// Set icons for the "places"
		inline void setCustomPlacesIcon(video::ITexture* texture) {
          if (texture) PlacesIconIdx = addIcon(texture);
          else PlacesIconIdx = -1;
          fillListBox();
        }
        
        /**
        \brief Sets whether directories can be chosen as the 'file' to open
        \param choosable - Whether the directory can be chosen
        */
        inline void setDirectoryChoosable(bool choosable) { IsDirectoryChoosable = choosable; }

		inline bool isComplete() {return usecomplete;}

   protected:

        /**
        \brief Returns true if the file extension is one of the registered filters
        \param s - the string to be checked
        \return a bool
        */
        bool matchesFileFilter(core::stringw s);
        /**
        \brief Returns true if the file extension matches the specified filter
        \param s - the string to be checked
        \param f - the filter to check for
        \return a bool
        */
        bool matchesFileFilter(core::stringw s, core::stringw f);
        
      /**
        \brief Fills the listbox with files.
        */
      void fillListBox();

      /**
        \brief Sends the event that the file has been selected.
        */
      void sendSelectedEvent();

      /**
        \brief Sends the event that the file choose process has been canceld
        */
      void sendCancelEvent();
      
      u32 addIcon(video::ITexture* texture);
            
      /** Struct to describe file filters to use when displaying files in directories */
        struct SFileFilter {
          /*
          \brief Constructor
          \param name - The name/description of the filter
          \param filter - The file extension required
          \param texture - The texture to use as an icon for the file type
          */
          SFileFilter(wchar_t* name, wchar_t* filter, video::ITexture* texture) {
            FilterName = name;
            FileExtension = filter;
            FileIcon = texture;
            FileIconIdx = 0;
          }
          void operator=(const SFileFilter& other) {
            FilterName = other.FilterName;
            FileExtension = other.FileExtension;
            FileIcon = other.FileIcon;
            FileIconIdx = other.FileIconIdx;
          }
          core::stringw FilterName;
          core::stringw FileExtension;     
          video::ITexture* FileIcon;
          u32 FileIconIdx;
        };

	 io::path FileDirectory;

      core::position2d<s32> DragStart;
      bool Dragging;
      bool IsDirectoryChoosable;
      s32 FileIconIdx;
      s32 DirectoryIconIdx;
	  s32 PlacesIconIdx;
      IGUIButton* CloseButton;
      IGUIButton* OKButton;
      IGUIButton* CancelButton;
      IGUIEditBox* FileNameText;
	  IGUIEditBox* PathNameText;
      IGUIListBox* FileBox;
	  IGUITreeView* TreeBox;
	  IGUIListBox* PlacesBox;
	  IGUIListBox* PlacesBoxReal;
      IGUIComboBox* DriveBox;
      IGUIComboBox* FilterComboBox;
      IGUIElement* EventParent;
      IGUISpriteBank* SpriteBank;
      io::IFileSystem* FileSystem;
      io::IFileList* FileList;
      core::array<SFileFilter> FileFilters;
      E_FILESELECTOR_TYPE DialogType;
      core::stringc prev_working_dir;
	  core::stringw fullpathname; 
      
      static s32 numFileSelectors;

	  bool usecomplete;
         
};

#endif /* INC_CGUIFILESELECTOR_H */