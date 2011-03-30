///////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/dataobj.h
// Purpose:     declaration of the wxDataObject class for Motif
// Author:      Julian Smart
// RCS-ID:      $Id: dataobj.h 67254 2011-03-20 00:14:35Z DS $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_X11_DATAOBJ_H_
#define _WX_X11_DATAOBJ_H_

// ----------------------------------------------------------------------------
// wxDataObject is the same as wxDataObjectBase under wxMotif
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxDataObject : public wxDataObjectBase
{
public:
    wxDataObject();

#ifdef __DARWIN__
    virtual ~wxDataObject() { }
#endif

    virtual bool IsSupportedFormat( const wxDataFormat& format, Direction dir = Get ) const;
};

#endif //_WX_X11_DATAOBJ_H_
