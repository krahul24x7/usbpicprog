/***************************************************************************
*   Copyright (C) 2008 by Frans Schreuder                                 *
*   usbpicprog.sourceforge.net                                            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef PICTYPE_H
#define PICTYPE_H

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/bitmap.h>

//#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

// forward declarations:
class wxXmlNode;
class wxDC;


// constants:

#define UPP_UNKNOWN_BITMAP_FILE wxT("unknown.png")
#define UPP_INDEX_FILE          wxT("index.xml")

#ifdef __WXMAC__
#define UPP_INDEX_PATH (((wxStandardPaths &)wxStandardPaths::Get()).GetExecutablePath() + _T("/xml_data/"))
#else
#define UPP_INDEX_PATH (wxStandardPaths::Get().GetDataDir() + wxFileName::GetPathSeparator())
#endif

#define UPP_DEFAULT_PIC_MODEL   wxT("18F2550")
#define UPP_DEFAULT_PIC         "P18F2550"

#define UPP_INVALID_PIC         (PicType())

#if wxCHECK_VERSION(2,8,10)		
#define _uT
#else
#define _uT wxT
#endif

// enums:

/**
    PicFamily indicates a programming algorith, each one is used by a certain 
    range of PICs (in some cases a single one!)
*/
typedef enum {
    P18F2XXX,P18FXX2,P16F87XA,P16F62XA,P16F62X,P12F629,P12F6XX,P16F84A,P16F81X,
    P16F7X,P16F7X7,P16F87X,P16F72,P16F87,P16F54,P16F57,P16F785,P16F59,P16F91X,P16F88X,
    P16C6XX,P16C55X,P16C7XX,P16C64X,P14000,P16C50X,P16C432,P17CXX,P17C7XX,
    P18FXX39,P18F6X2X,P18FXX80,P18F8410,P18F6XJXX,P18F45J10,P18F97J60,P18F1X30,P18FXX23,
    P18FXXK20,P24FJXXXGA0XX,P24FJXXXGA1,dsP30F,dsP33F,P24H,P10F200,P10F202,P12F61X,P16F716,
    UPP_INVALID_PICFAMILY

    // VERY IMPORTANT: when adding/removing enumeration values here, please
    //                 update also the PicType::GetFamilyFromString function!
} PicFamily;

/**
    The possible types of packages.
*/
typedef enum {
    PDIP,
    SOIC,
    SSOP,
    MQFP,
    TQFP,
    PLCC,
    UPP_INVALID_PACKAGETYPE

    // VERY IMPORTANT: when adding/removing enumeration values here, please
    //                 update also the ChipPackage::GetPackageTypeFromString function!
} PackageType;

/**
    The meaning of the voltage values stored inside the arrays of Pic class.
*/
typedef enum {
    MINIMUM = 0,
    NOMINAL,
    MAXIMUM,

    VOLTAGE_TYPE_MAX
} VoltageType;

	

/**
    Describes the package of a PIC (used only for informative purposes).
    
    In Piklab XML files this class holds the information stored in a @c package node.
*/
class ChipPackage
{
public:
    ChipPackage() : Type(UPP_INVALID_PACKAGETYPE) {}

    /**
        The name of the pins.
    
        The size of this array indicates the number of pins of this package.
        The name associated with some of the pins maybe the empty string or not.
    */
    wxArrayString PinNames;

    /**
        The type of this package.
    */
    PackageType Type;

    /**
        Draws this package on the given @a dc, making it fit inside the given
        size @a sz. In the center of the package the @a chipModel string is drawn.
    */
    void Draw(wxDC& dc, const wxSize& sz, const wxString& chipModel);

    /**
        Returns the name of this package.
    */
    wxString GetName() const
        { return GetStringFromPackageType(Type); }
        
    /**
        Returns the number of pins of this package.
    */
    unsigned int GetPinCount() const
        { return PinNames.GetCount(); }

    /**
        Returns true if the pin with index @a idx is involved in ICSP
        (in circuit serial programming).
    */
    bool IsICSPPin(unsigned int idx) const
    {
 #if wxCHECK_VERSION(2,8,10)			
        if (PinNames[idx].Contains("VDD") ||
            PinNames[idx].Contains("VSS") ||
            PinNames[idx].Contains("GND") ||
            PinNames[idx].Contains("ICSP") ||
            PinNames[idx].Contains("MCLR") ||
            PinNames[idx].Contains("PGC") ||
            PinNames[idx].Contains("PGD"))
#else			
		if (PinNames[idx].Contains(wxT("VDD")) ||
            PinNames[idx].Contains(wxT("VSS")) ||
            PinNames[idx].Contains(wxT("GND")) ||
            PinNames[idx].Contains(wxT("ICSP")) ||
            PinNames[idx].Contains(wxT("MCLR")) ||
            PinNames[idx].Contains(wxT("PGC")) ||
            PinNames[idx].Contains(wxT("PGD")))			
#endif			
            return true;
        return false;
    }

public:     // static
    /**
        Returns the PackageType for the given package type name or 
        @c UPP_INVALID_PACKAGETYPE on error.
    */
    static PackageType GetPackageTypeFromString(const wxString& str);

    /**
        Returns the name for the given PackageType.
    */
    static wxString GetStringFromPackageType(PackageType type);
    
    
protected:  // internal utils
    /**
        Flags to be used with DrawPins().
    */
    enum DrawPinsFlags
    {
        /**
            Specifies that the pins are drawn from the last one back to the first one.
        */
        DRAWPIN_INVERTED_ORDER = 1,

        /**
            Specifies that the pin numbers should be drawn inside the pin rects,
            rather than drawing them next to the rects.
        */
        DRAWPIN_NUMBERS_INSIDE_PINS = 2
    };

    /**
        Draws a row or column of pins.
        
        More precisely, this function draws a rectangle of appropriate size
        representing the pin (as seen from above the package), the pin number next
        to it and finally the pin name next to it (on the opposite side of the
        pin number).
        
        @param dc
            The DC where the pins are drawn
        @param pt
            The location at which the strip of pins should start
        @param PackageLen
            The len of the package (in pixels) in the direction specified by @a dir
        @param FirstPin
            The index of the first pin to draw
        @param LastPin
            The index of the last pin to draw + 1
        @param flags
            A combination of the enum values of DrawPinsFlags
        @param dir
            The direction of the strip of pins.
            If wxLEFT or wxRIGHT is given, then a vertical column of pins is drawn
            with the pin names drawn on the left of the pins (first case) or on
            the right of the pins (second case).
            If wxTOP or wxBOTTOM is given, then an horizontal row of pins is drawn
            with the pin names drawn above the pins (first case) or below them
            (second case). 
            The pin names in wxTOP/wxBOTTOM case are printed rotated by 90 degrees.
    */
    void DrawPins(wxDC& dc, const wxPoint& pt, unsigned int PackageLen,
                unsigned int FirstPin, unsigned int LastPin, int flags,
                wxDirection dir);
};

/**
    Describes a configuration value for a ConfigMask object.
    
    In Piklab XML files this class holds the information stored in a
    <tt>config -> mask -> value</tt> node:
    
    @code
    <config ...>
        <mask ...>
            <value value="0x0000" name="LP" />
        </mask>
    </config>
    @endcode
*/
class ConfigValue
{
public:
    ConfigValue() : Value(0) {}

    /**
        The name of this configuration value; e.g. "XT" to indicate that the oscillator
        of the PIC (if the ConfigMask object which contains us is called "FOSC")
        should use a quartz crystal.
    */
    wxString Name;
    
    /**
        The value that the set of bits grouped by the ConfigMask object which contains us
        should assume when the user selects this configuration value.
        
        E.g. for PIC18F2550 the bits 2-0 in the first config word (named PLLDIV2, PLLDIV1
        and PLLDIV0) should assume configuration value "101" when the user selects
        "Divide by 6 (24 MHz oscillator input)".
    */
    unsigned long Value;

    // leave out cname, sdcc_cname
};

/**
    Describes a configuration "mask".
    
    This is a set of bits inside a configuration word (represented by ConfigWord class)
    dedicated to the same purpose (e.g. the configuration of the PIC oscillator).
    
    In Piklab XML files this class holds the information stored in a
    <tt>config -> mask</tt> node:
    
    @code
    <config ...>
        <mask name="FOSC"  value="0x0003">
            ...
        </mask>
    </config>
    @endcode
*/
class ConfigMask
{
public:
    ConfigMask() {}

    /**
        The name of this set of configuration bits.
    */
    wxString Name;
    
    /**
        The various configuration values that this set of bits can assume.
    */
    vector<ConfigValue> Values;

    // leave out 'value' from Piklab XML files; we calculate the mask on the 
    // fly in GetMask()
    
    /**
        Returns the array containing the user-friendly names of the available
        configuration values.
    */
    wxArrayString GetStringValues(bool includeValues = true) const
    {
        wxArrayString ret;
        for (unsigned int i=0; i<Values.size(); i++)
        {
            if (includeValues)
			{
#if wxCHECK_VERSION(2,8,10)				
                ret.Add(wxString::Format("%s [0x%X]", Values[i].Name, (unsigned int) Values[i].Value));
#else
				ret.Add(wxString::Format(wxT("%s [0x%X]"), Values[i].Name.c_str(), (unsigned int) Values[i].Value));
#endif
			}
            else
                ret.Add(Values[i].Name);
        }
        return ret;
    }

    /**
        Computes the mask associated with the set of bits for this configuration mask
        and returns it.
    */
    unsigned long GetMask() const
    {
        int tmpConfigMask=0;
        for (unsigned int j=0; j<Values.size(); j++)
            tmpConfigMask |= Values[j].Value;

        return tmpConfigMask;
    }

    /**
        Returns the number of bits grouped by this configuration mask.
        
        Note that the returned value is 1-based and not 0-based.
    */
    unsigned int GetBitSize() const
    {
        unsigned int nbits = 0;
        unsigned long tmp = GetMask();
        
        for (; tmp != 0; )
        {
            if ((tmp%2) == 1) 
                nbits++;    // the last bit of 'tmp' is set to 1
                
            // analyze next bit:
            tmp >>= 1;
        }
        
        // nbits is the number of bits set to 1 of our config mask
        return nbits;
    }
};

/**
    Describes a word of configuration bits.
    
    This is usually 8 bits grouped in some ConfigMask objects.

    In Piklab XML files this class holds the information stored in a
    <tt>config</tt> node:

    @code
    <config offset="0x0" name="CONFIG1L" wmask="0xFF" bvalue="0x00">
        ...
    </config>
    @endcode
*/
class ConfigWord
{
public:
    ConfigWord() : Offset(0) {}

    /**
        The offset of this configuration word inside the configuration memory area of the PIC.
    */
    unsigned long Offset;
    
    /**
        The name of this configuration word.
    */
    wxString Name;
    
    /**
        The different sets of bits in which this configuration word is divided.
    */
    vector<ConfigMask> Masks;

    // leave out 'wmask', 'bvalue' from Piklab XML files; we calculate the masks on the 
    // fly in GetMask()
    
    /**
        Returns the mask for this configuration word.
        This is obtained OR-ing all possible values for all our ConfigMask objects.
    */
    unsigned long GetMask() const
    {
        int tmpConfigMask=0;
        for (unsigned int j=0; j<Masks.size(); j++)
            tmpConfigMask |= Masks[j].GetMask();
        
        return tmpConfigMask;
    }
    
    /**
        Returns the number of bits grouped by this configuration word.
        
        Note that the returned value is 1-based and not 0-based.
    */
    unsigned int GetBitSize() const
    {
        unsigned int nbits;
        unsigned long tmp = GetMask();
        
        for (nbits=0; tmp != 0; nbits++)
            tmp >>= 1;
        
#ifdef __WXDEBUG__
        // coherency check:
        unsigned int acc = 0;
        for (unsigned int i=0; i<Masks.size(); i++)
            acc += Masks[i].GetBitSize();
        wxASSERT(acc <= nbits);
#endif
        
        // nbits is the index of the MSB set to 1 of our mask;
        // note that for a mask of e.g. "3FF0" this function returns 14 (i.e. the number
        // of bits required to e.g. store this configuration word) and not 10
        // (i.e. the number of bits set to 1)
        return nbits;
    }
};

/**
    This class represents a PIC device type.

    The PicType class stores all properties of a PIC, but does @b NOT store the
    code, config and data memory areas (see HexFile class for that).
    
    The info stored in this class are initialized at run-time using one of
    FindPIC() static functions and then are not modified anymore.
*/
class PicType
{
public:
    /**
        Default constructor.
        
        To make a PicType instance valid (so that ok() return true) initialize
        a PicType assigning it the return value of FindPIC() function.
    */
    PicType() : CodeSize(0), ConfigAddress(0), DataAddress(0), DataSize(0), ConfigSize(0), 
                picFamily(UPP_INVALID_PICFAMILY), DevId(0), DevIdMask(0), MinFreq(0), MaxFreq(0)
    {
        for (unsigned int i=0; i<VOLTAGE_TYPE_MAX; i++)
            WorkVoltages[i] = ProgVoltages[i] = 0.0;
    }

    // public members to avoid lots of getters/setters:

    /// Name of the PIC model described by this Pic instance.
    /// This string always starts with "P" (not "PIC") prefix.
    wxString Name;

    /// Size of the code memory area (in word units??)
    unsigned int CodeSize;

    /// Size of the configuration memory area (in word units??)
    unsigned int ConfigSize;

    /// Size of the data memory area (in word units??)
    unsigned int DataSize;

    /// Address for the configuration memory area (in word units??)
    unsigned int ConfigAddress;

    /// Address for the data memory area (in word units??)
    unsigned int DataAddress;

    /// The family to which this PIC belongs.
    PicFamily picFamily;

    /// The device ID for this type of PIC.
    unsigned int DevId;

    /// The bitmask to be used when comparing/manipulating #DevId.
    unsigned int DevIdMask;

    /// The array of configuration blocks.
    vector<ConfigWord> ConfigWords;

    /// Package descriptors.
    vector<ChipPackage> Package;

    /// The programming voltages; see VoltageType enum.
    float ProgVoltages[VOLTAGE_TYPE_MAX];

    /// The working voltages; see VoltageType enum.
    float WorkVoltages[VOLTAGE_TYPE_MAX];

    /// The minimum & maximum frequencies for this PIC.
    float MinFreq, MaxFreq;

	/// Two backup calibration registers for 12F629 devices
	unsigned int OscCal, BandGap;

	struct CheckSumType
    {
		unsigned int bChecksumCpOff;
		unsigned int cChecksumCpOff;
		unsigned int bChecksumCpAll;
		unsigned int cChecksumCpAll;
    } CheckSums;

    /** Is this PIC instance correctly initialized? */
    bool ok() const
        { 	return !Name.empty() && picFamily != UPP_INVALID_PICFAMILY; 
            /* TODO: check for more members to be non-NULL */ }

    /** Returns true if the PIC is a 14bit device (e.g. Pic12, PIC16). */
    bool is14Bit() const
 #if wxCHECK_VERSION(2,8,10)			
        { return (Name.find("P10")==0)||(Name.find("P12")==0)||(Name.find("P16")==0); }
#else	
		{ return (Name.find(wxT("P10"))==0)||(Name.find(wxT("P12"))==0)||(Name.find(wxT("P16"))==0); }
#endif	

    /** Returns true if the PIC is a 16bit device (e.g. Pic18). */
    bool is16Bit() const
#if wxCHECK_VERSION(2,8,10)			
        { return (Name.find("P18F")==0); }
#else
		{ return (Name.find(wxT("P18F"))==0); }
#endif	

	    /** Returns true if the PIC is a 24bit device (e.g. dsPIC). */
    bool is24Bit() const
#if wxCHECK_VERSION(2,8,10)		
        { return (Name.find("P30F")==0); }
#else	
		{ return (Name.find(wxT("P30F"))==0); }
#endif	
	
    /** Returns the PIC name which starts with "PIC" instead of "P". */
    wxString GetExtName() const
        {
            if (Name.empty()) return Name;
#if wxCHECK_VERSION(2,8,10)				
            return "PIC" + Name.substr(1); 
#else
			return wxT("PIC") + Name.substr(1); 
#endif			
        }
        
    /** Returns the current PIC name as a wxString. */
    wxString getPicName() const
        { return Name; }


    // static utilities

    /**
        Returns a PicType instance describing the PIC model named @a picType.

        If there's no supported PIC with the given name, then the instance
        returned will return false when calling ok() on it.
    */
    static PicType FindPIC(wxString picType);

    /**
        Returns a PicType instance describing the PIC model whose device id is @a devId.

        If there's no supported PIC with the given device id, then the instance
        returned will return false when calling ok() on it.
    */
    static PicType FindPIC(unsigned int devId);

    /**
        Returns an array (vector) of strings containing all names of supported PICs.
    */
    static const wxArrayString& getSupportedPicNames();

    /** 
        Loads the @c UPP_INDEX_FILE XML file and initializes the internal static array
        which describes all supported PICs.

        This needs to be called only once at program startup.
    */
    static bool Init();
    
    /**
        Clears all internal static variables.
        
        This needs to be called only once at program exit.
    */
    static void CleanUp();

private:        // data
    /**
        An internal private structure used to store the info extracted by
        the @c UPP_INDEX_FILE file.
    */
    typedef struct
    {
        /// This is the pic name @e without the initial "P" or "PIC" prefix
        wxString name;
        
        PicFamily picFamily;
        unsigned int devIdMask;
        unsigned int devId;

        /// This is the full cached PicType instance. 
        /// If pic->ok()==false or pic==NULL, it means the relative XML file still 
        /// needs to be loaded (see LoadPIC()).
        PicType* pic;
    } PicIndexInfo;

    /**
        The names of the supported PICs.
        This is loaded in Init() from @c UPP_INDEX_FILE and is never changed then.
    */
    static vector<PicIndexInfo> s_arrSupported;

    /**
        The PicIndexInfo structure relative to the @c UPP_DEFAULT_PIC.
    */
    static PicIndexInfo s_default;

private:    // utilities
    
    /**
        Loads the given XML file.

        @note Does not log any error.
    */
    bool LoadPIC(PicType::PicIndexInfo& indexInfo);

    /**
        Loads the Piklab XML for the given PIC name.
        
        @param picName
            The name of the PIC whose data must be loaded.
            This string must @b not start with 'P' or 'PIC' but directly with the 
            model number (e.g. '16F84A').

        @note Does not log any error.

        @return UPP_INVALID_PIC on failure, or a valid PicType otherwise.
    */
    static PicType LoadPiklabXML(const wxString& picName);
    
    /**
        Loads the Piklab XML identified by the given @a fileName.
        
        @note Does not log any error.

        @return UPP_INVALID_PIC on failure, or a valid PicType otherwise.
    */
    static PicType LoadPiklabXMLFile(const wxString& fileName);

    /**
        Returns the range indicated by the given XML node.
    */
    static int GetRange(const wxXmlNode* p);

    /**
        Returns the PicFamily for the given family name or 
        @c UPP_INVALID_PICFAMILY on error.
    */
    static PicFamily GetFamilyFromString(const wxString& str);

	
};

#if wxCHECK_VERSION(2,9,0)
#else	//this function is only implemented in wxWidgets 2.9.0, for 2.8.x compatibility, implement it here...
	wxArrayString wxSplit(const wxString& str,
                                       const wxChar sep,
                                       const wxChar escape = wxT('\\'));
#endif	

#endif  //PICTYPE_H
