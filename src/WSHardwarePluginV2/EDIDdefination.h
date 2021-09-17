#pragma once
namespace Hardware
{
	namespace Monitor
	{
#pragma pack(push,1)
		struct EDID_Detailed_Timing_Descriptor
		{
			WORD						Pixel_Clock;	//Pixel clock in 10 kHz units. (0.01–655.35 MHz, little-endian)
			BYTE						Horizental_active_pixels;	//Horizontal active pixels 8 lower bits (0–4095)
			BYTE						Horizontal_blanking_pixels;	//	Horizontal blanking pixels 8 lower bits (0–4095) End of active to start of next active.
			union
			{
				struct
				{
					BYTE				Horizontal_blanking_pixels : 4;	//upper 4bits
					BYTE				Horizental_active_pixels : 4;	//upper 4bits
				}bits;
				BYTE					Horizental_pixels_upperbits;
			}				Horizental_pixels_upperbits;
			BYTE						Vertical_active_pixels; //Vertical active lines 8 lower bits (0–4095)
			BYTE						Vertical_blanking_pixels;	//Vertical blanking lines 8 lower bits (0–4095)
			union
			{
				struct
				{
					BYTE				Vertical_blanking_pixels : 4;		//Bits 3–0	Vertical blanking lines 4 upper bits
					BYTE				Vertical_active_pixels : 4;			//Bits 7–4	Vertical active lines 4 upper bits
				}bits;
				BYTE					Vertical_lines_msbits;
			}				Vertical_lines_msbits;
			BYTE						Horizontal_front_porch;	//Horizontal front porch (sync offset) pixels 8 lower bits (0–1023) From blanking start
			BYTE						Horizontal_sync_pulse_width;	//	Horizontal sync pulse width pixels 8 lower bits (0–1023)
			union
			{
				struct
				{
					BYTE				Vertical_front_porch : 4;		//Bits 3–0	Vertical sync pulse width lines 4 lower bits (0–63)
					BYTE				Vertical_sync_pulse_width : 4;		//Bits 7–4		Vertical front porch (sync offset) lines 4 lower bits (0–63)
				}bits;
				BYTE					Vertical_sync_lsbits;
			}				Vertical_sync_lsbits;
			union
			{
				struct
				{
					BYTE				Vertical_sync_pulse_width : 2;	//Bits 1–0	Vertical sync pulse width lines 2 upper bits
					BYTE				Vertical_front_porch : 2;	//Bits 3–2	Vertical front porch (sync offset) lines 2 upper bits
					BYTE				Horizontal_sync_pulse_width : 2;	//Bits 5–4	Horizontal sync pulse width pixels 2 upper bits
					BYTE				Horizontal_front_porch : 2;	//	Bits 7–6	Horizontal front porch (sync offset) pixels 2 upper bits
				}bits;
				BYTE					H_V_sync_msbits;
			}				sync_msbits;
			BYTE						Horizontal_image_size;	//	Horizontal image size, mm, 8 lower bits (0–4095 mm, 161 in)
			BYTE						Vertical_image_size;	//	Vertical image size, mm, 8 lower bits (0–4095 mm, 161 in)
			union
			{
				struct
				{
					BYTE				Vertical_image_size : 4;	//	Bits 3–0	Vertical image size, mm, 4 upper bits
					BYTE				Horizontal_image_size : 4;	//	Bits 7–4	Horizontal image size, mm, 4 upper bits
				}bits;
				BYTE					Image_Size_msbits;
			}				Image_Size_msbits;
			BYTE						Horizontal_border_pixels;	//	Horizontal border pixels (one side; total is twice this)
			BYTE						Vertical_border_lines;	//	Vertical border lines (one side; total is twice this)
			BYTE						Features_bitmap;
			/************************************************************************
			Bit 7	Interlaced
			Bits 6–5	Stereo mode: 00=No stereo; other values depend on bit 0:
			Bit 0=0: 01=Field sequential, sync=1 during right; 10=similar, sync=1 during left; 11=4-way interleaved stereo
			Bit 0=1: 01=Right image on even lines; 10=Left image on even lines; 11=side-by-side
			Bit 4=0	Analog sync.
			If set, the following bit definitions apply:
			Bit 3	Sync type: 0=Analog composite; 1=Bipolar analog composite
			Bit 2	VSync serration (HSync during VSync)
			Bit 1	Sync on all 3 RGB lines (else green only)
			Bits 4-3=10	Digital composite (on HSync).
			If set, the following bit definitions apply:
			Bit 2	Vertical sync polarity (0=negative, 1=positive)
			Bit 1	reserved
			Bits 4-3=11	Digital separate sync.
			If set, the following bit definitions apply:
			Bit 2	VSync serration (HSync during VSync)
			Bit 1	Horizontal Sync polarity (0=negative, 1=positive)
			Bit 0	2-way line-interleaved or side-by-side interleaved stereo, if bits 6–5 are not 00.
			*************************************************************************/
		};

		//When used for another descriptor, the pixel clock and some other bytes are set to 0:

		struct EDID_Other_Monitor_Descriptors
		{
			BYTE					Reserved[3];	// must be 0
			BYTE					Descriptor_Type;	//	Descriptor type. FA–FF currently defined. 00–0F reserved for vendors.
			BYTE					Reserve; // must be 0
			BYTE					Defined_by_descriptor_type;
		};

		/************************************************************************

		Currently defined descriptor types are:
		0xFF: Display serial number (ASCII text)
		0xFE: Unspecified text (ASCII text)
		0xFD: Display range limits. 6- or 13-byte (with additional timing) binary descriptor.
		0xFC: Display name (ASCII text).
		0xFB: Additional white point data. 2× 5-byte descriptors, padded with 0A 20 20.
		0xFA: Additional standard timing identifiers. 6× 2-byte descriptors, padded with 0A.
		0xF9 Display Color Management (DCM).
		0xF8 CVT 3-Byte Timing Codes.
		0xF7 Additional standard timing 3.
		0x10 Dummy identifier.
		00-0x0Fh Manufacturer reserved descriptors.
		************************************************************************/

		struct EDID_Unspecified_Text
		{
			DWORD					Standard_header;	// byte 3 = 0xFE.
			BYTE					Reseerved;	//00
			BYTE					Unspecified_Text[13];
		};

		struct GTF_SECONDARY_CURVE
		{
			BYTE						Reserved;	//	Reserved, must be 0.
			BYTE						Start_frequency;	//Start frequency for secondary curve, divided by 2 kHz (0–510 kHz)
			BYTE						GTF_C_Value;	//GTF C value, multiplied by 2 (0–127.5)
			BYTE						GTF_M_Value[2];	//GTF M value (0–65535, little-endian)
			BYTE						GTF_K_Value;	//GTF K value (0–255)
			BYTE						GTF_J_Value;	//GTF J value, multiplied by 2 (0–127.5)
		};

		struct CVT_SUPPORT
		{
			union
			{
				struct
				{
					BYTE				CVT_minor_version : 4;	//CVT minor version (0-15)
					BYTE				CVT_major_version : 4;	//CVT major version (1-15)
				}bits;
				BYTE					CVT_Version;
			}				CVT_Version;
			union
			{
				struct
				{
					BYTE				Maximum_active_pixels : 2;	//Bits 1–0	Maximum active pixels per line, 2-bit msb
					BYTE				Additional_clock_precision : 6;	//	Bits 7–2	Additional clock precision in 0.25 MHz increments (to be subtracted from byte 9 Maximum pixel clock rate)
				}bits;
				BYTE					CVT_pixels_And_Clock;
			}				CVT_pixels_And_Clock;
			BYTE						Maximum_active_pixels;	//Maximum active pixels per line, 8-bit lsb (no limit if 0)
			union
			{
				struct
				{
					BYTE				Reserved : 3;
					BYTE				_15_9 : 1;
					BYTE				_5_4 : 1;
					BYTE				_16_10 : 1;
					BYTE				_16_9 : 1;
					BYTE				_4_3 : 1;
				}bits;
				BYTE					Aspect_ratio_bitmap;
			}				Aspect_ratio_bitmap;
			union
			{
				struct
				{
					BYTE				Reserved : 3;
					BYTE				CVT_Standard_Blanking : 1;
					BYTE				CVTRB_reduced_blanking : 1;
					BYTE				Aspect_ratio_preference : 3;	// 000=4:3, 001=16:9, 010=16:10, 011=5:4, 100=15:9
				}bits;
				BYTE					CVT_Blanking;
			}				CVT_Blanking;
			union
			{
				struct
				{
					BYTE				Reserved : 4;	//	Reserved, must be 0.
					BYTE				Vertical_stretch : 1;
					BYTE				Vertical_shrink : 1;
					BYTE				Horizontal_stretch : 1;
					BYTE				Horizontal_shrink : 1;
				}bits;
				BYTE					Scaling_support_bitmap;
			}				Scaling_support_bitmap;
			BYTE						Preferred_vertical_refresh_rate;	// (1–255)
		};

		struct EDID_Display_Range_Limits_Descriptor
		{
			DWORD						Standard_header; //byte 3 = 0xFD.
			union
			{
				struct
				{
					BYTE				Vertical_rate_offsets : 2;	// 00=None, 10=+255 Hz for Max rate, 11=+255 Hz for Max and Min rates
					BYTE				Horizontal_rate_offsets : 2;// 00=None, 10=+255 kHz for Max rate, 11=+255 kHz for Max and Min rates
					BYTE				Unused : 4;	//Unused, must be 0.
				};
				BYTE					Offsets_for_display_range_limits;
			}				Offsets_for_display_range_limits;
			BYTE						Minimum_vertical_field_rate;	//Minimum vertical field rate (1–255 Hz) (256–512 Hz if offset)
			BYTE						Maximum_vertical_field_rate;	//Maximum vertical field rate (1–255 Hz) (256–512 Hz if offset)
			BYTE						Minimum_horizontal_field_rate;	//Minimum horizontal field rate (1–255 kHz) (256–512 kHz if offset)
			BYTE						Maximum_horizontal_field_rate;	//Maximum horizontal field rate (1–255 kHz) (256–512 kHz if offset)
			BYTE						Maximum_pixel_clock_rate;	//	Maximum pixel clock rate, rounded up to 10 MHz multiple (10–2550 MHz)
			BYTE						Extended_timing_information_type;
			/*	Extended timing information type:
			00: Default GTF(when Basic display parameters byte 24 bit 0 is set.
			01: No timing information.
			02 : Secondary GTF supported, parameters as follows.
			04 : CVT(when Basic display parameters byte 24 bit 0 is set), parameters as follows.*/
			union
			{
				GTF_SECONDARY_CURVE			GTF_Secondary_curve;
				CVT_SUPPORT					CVT;
				BYTE						Video_timing_parameters[7];
			}Video_timing_parameters;//if byte 10 is 01 or 02, padded with 0A 20 20 20 20 20 20.
		};

		struct EDID_Descriptor_Common_String
		{
			DWORD					Standard_header;	// byte 3 = 0xFC.
			BYTE					Reseerved;	//00
			BYTE					Display_Name[13];
		};

		struct EDID_Additional_White_Point_Descriptor
		{
			DWORD					Standard_header; //Standard header, byte 3 = 0xFB.
			BYTE					Reserved;
			BYTE					White_Point_Index_Number;	//	White point index number (1–255) Usually 1; 0 indicates descriptor not used
			union
			{
				struct
				{
					BYTE				Y_Value : 2;	// 	White point y value least-significant 2 bits
					BYTE				X_Value : 2;// 	White point x value least-significant 2 bits
					BYTE				Unused : 4;	//Unused, must be 0.
				};
				BYTE					White_Point_CIE_xy;
			}			White_Point_CIE_xy;
			BYTE					White_X_Value_Significant; // White point x value most significant 8 bits(like EDID byte 27)
			BYTE					White_Y_Value_Significant;//White point y value most significant 8 bits(like EDID byte 28)
			BYTE					Datavalue;	//datavalue = (gamma−1)×100 (1.0–3.54, like EDID byte 23)
			BYTE					Second_Descriptor[5];	//Second descriptor, like above. Index number usually 2.
			BYTE					Unused[3];	//Unused, padded with 0A 20 20.
		};

		struct EDID_StandardTimingIdentifierDefinition //tag #FAh
		{
			DWORD Tag;
			std::uint16_t Timing[6];
			std::uint8_t LineFeed;
		};

		struct EDID_Color_Management_Data_Descriptor
		{
			DWORD					Standard_header;	// byte 3 = 0xF9.
			BYTE					Reserved;
			BYTE					Version;	//03
			BYTE					RedA3lsb;
			BYTE					Reda3msb;
			BYTE					Reda2lsb;
			BYTE					Reda2msb;
			BYTE					GreenA3lsb;
			BYTE					GreenA3msb;
			BYTE					GreenA2lsb;
			BYTE					GreenA2msb;
			BYTE					BlueA3lsb;
			BYTE					BlueA3msb;
			BYTE					BlueA2lsb;
			BYTE					BlueA2msb;
		};

		struct EDID_CVT_Timing_Codes_Descriptor
		{
			DWORD					Standard_header;	// byte 3 = 0xF8.
			BYTE					Reserved;
			BYTE					Version;	//03
												//CVT timing descriptor #1
			BYTE					Addressable_lsb;//Addressable lines 8-bit lsb
			union
			{
				struct
				{
					BYTE			Unused : 2;	//Unused, must be 0.
					BYTE			Preferred_vertical_rate : 2;	//: 00=50 Hz, 01=60 Hz, 10=75 Hz, 11=85 Hz
					BYTE			Addressable_msb : 4;	//Addressable lines 4-bit msb
				}bits;
				BYTE				Data;
			}			Preferred_vertical_rate;
			union
			{
				struct
				{
					BYTE			CVT_60HZ_reduced_blanking : 1;
					BYTE			CVT_85HZ : 1;
					BYTE			CVT_75HZ : 1;
					BYTE			CVT_60HZ : 1;
					BYTE			CVT_50HZ : 1;
					BYTE			Aspect_ratio : 2;	//00=16:10; 01=4:3; 10=5:4; 11=16:9
					BYTE			Unused : 1;	//Unused, must be 0.
				}bits;
				BYTE				Data;
			}			Vertical_rate_bitmap;
			BYTE					CVT_Timing_Descriptor_2[3];
			BYTE					CVT_Timing_Descriptor_3[3];
			BYTE					CVT_Timing_Descriptor_4[3];
		};

		struct EDID_Additional_Standard_Timings
		{
			DWORD					Standard_header;	// byte 3 = 0xF7.
			BYTE					Reserved;
			BYTE					Version;	//10
			union
			{
				struct
				{
					BYTE			Is1152x864_85hz : 1;
					BYTE			Is1024x768_85hz : 1;
					BYTE			Is800x600_85hz : 1;
					BYTE			Is848x480_60hz : 1;
					BYTE			Is640x480_85hz : 1;
					BYTE			Is720x400_85hz : 1;
					BYTE			Is640x400_85hz : 1;
					BYTE			Is640x350_85hz : 1;
				}bits;
				BYTE				Data;
			}			resolution_1;
			union
			{
				struct
				{
					BYTE			Is1280x1024_85hz : 1;
					BYTE			Is1280x1024_60hz : 1;
					BYTE			Is1280x960_85hz : 1;
					BYTE			Is1280x960_60hz : 1;
					BYTE			Is1280x768_85hz : 1;
					BYTE			Is1280x768_75hz : 1;
					BYTE			Is1280x768_60hz : 1;
					BYTE			Is1280x768_60hz_CVTRB : 1;
				}bits;
				BYTE				Data;
			}			resolution_2;
			union
			{
				struct
				{
					BYTE			Is1440x1050_75hz : 1;
					BYTE			Is1440x1050_60hz : 1;
					BYTE			Is1440x1050_60hz_CVTRB : 1;
					BYTE			Is1440x900_85hz : 1;
					BYTE			Is1440x900_75hz : 1;
					BYTE			Is1440x900_60hz_CVTRB : 1;
					BYTE			Is1280x768_60hz : 1;
					BYTE			Is1360x768_60hz_CVTRB : 1;
				}bits;
				BYTE				Data;
			}			resolution_3;
			union
			{
				struct
				{
					BYTE			Is1600x1200_70hz : 1;
					BYTE			Is1600x1200_65hz : 1;
					BYTE			Is1600x1200_60hz : 1;
					BYTE			Is1680x1050_85hz : 1;
					BYTE			Is1680x1050_75hz : 1;
					BYTE			Is1680x1050_60hz : 1;
					BYTE			Is1680x1050_60hz_CVTRB : 1;
					BYTE			Is1440x1050_85hz : 1;
				}bits;
				BYTE				Data;
			}			resolution_4;
			union
			{
				struct
				{
					BYTE			Is1920x1200_60hz : 1;
					BYTE			Is1920x1200_60hz_CVTRB : 1;
					BYTE			Is1856x1392_75hz : 1;
					BYTE			Is1856x1392_60hz : 1;
					BYTE			Is1792x1344_75hz : 1;
					BYTE			Is1792x1344_60hz : 1;
					BYTE			Is1600x1200_85hz : 1;
					BYTE			Is1600x1200_75hz : 1;
				}bits;
				BYTE				Data;
			}			resolution_5;
			union
			{
				struct
				{
					BYTE			Unused : 4;
					BYTE			Is1920x1440_75hz : 1;
					BYTE			Is1920x1440_60hz : 1;
					BYTE			Is1920x1200_85hz : 1;
					BYTE			Is1920x1200_75hz : 1;
				}bits;
				BYTE				Data;
			}			resolution_6;
			BYTE					Unused[6];	//must be 0
		};

		/// @brief EDID的通用信息
		struct EDIDCommon
		{
			std::uint64_t Header;											//BYTE 0-7 Fixed header pattern: 00 FF FF FF FF FF FF 00
			struct
			{
				union
				{
					struct
					{
						std::uint16_t _Third : 5;
						std::uint16_t _Second : 5;
						std::uint16_t _First : 5;
						std::uint16_t Reserved : 1;
					}bits;
					std::uint16_t Manufacturer;
				}ManufacturerID;											//Byte 8-9 Manufacturer ID
				std::uint16_t Productcode;									//Byte 10-11 Manufacturer product code. 16-bit number, little-endian.
				std::uint32_t SerialNumber;									//Byte 12-15 Serial number. 32 bits, little endian.
				std::uint8_t Week;											//Byte16 Week of manufacture, or model year flag. Week numbering is not consistent between manufacturers.
				std::uint8_t Year;											//Byte17 Year of manufacture, less 1990 (1990–2245). If week=255, it is the model year instead.
			}Identification;

			struct
			{
				std::uint8_t Version;										///<Byte18 	EDID version such as 1.3 , usually Byte18 = 1 Byte19 = 3
				std::uint8_t Revision;										///<Byte19 	EDID Revision such as 1.3 , usually Byte18 = 1 Byte19 = 3
			}EDIDVersion;

			struct
			{
				std::uint8_t VideoInputParameters;						///<Byte20 Video input parameters bitmap
				std::uint8_t HorizontalScreenSize;						///<Byte21 Horizontal screen size, in centimetres (range 1-255). If vsize=0, landscape aspect ratio (range 1.00-3.54), datavalue = (AR×100)-99 (example: 16:9 = 79, 4:3 = 34)
				std::uint8_t VerticalScreenSize;						///<Byte22 Vertical screen size, in centimetres. If hsize=0, portrait aspect ratio (range 0.28-0.99), datavalue = (100/AR)-99 (example: 9:16 = 79, 3:4 = 34). If either byte is 0, screen size and aspect ration are undefined (e.g. projector)
				std::uint8_t DisplayGamma;								///<Byte23 Display gamma, factory default (range 1.00–3.54), datavalue = (gamma×100)-100 = (gamma−1)×100. If 225, gamma is defined by DI-EXT block.
				std::uint8_t Features;									///<Byte24 	Supported features bitmap
			}BasicDisplayParametersAndFeatures;

			struct
			{
				std::uint8_t RedAndGreen;								///< Byte25 	Red and green least-significant bits (2−9, 2−10)
				std::uint8_t BlueAndWhite;								///< Byte26 	Blue and white least-significant 2 bits
				std::uint8_t RedxValue;									///< Byte27 Red x value most significant 8 bits (2−1,...,2−8). 0–255 encodes fractional 0–0.996 (255/256); 0–0.999 (1023/1024) with lsbits
				std::uint8_t RedyValue;									///< Byte28 Red y value most significant 8 bits
				std::uint8_t GreenxyValue[2];							///< Byte29-30 Green x and y value most significant 8 bits
				std::uint8_t BluexyValue[2];							///< Byte31-32 Blue x and y value most significant 8 bits
				std::uint8_t WhitexyValue[2];							///< Byte33-34 White x and y value most significant 8 bits
			}ColorCharacteristics;

			struct EstablishedTimingSection
			{
				std::uint8_t EstablishedTiming1;								///<Byte35 Established Timings I
				std::uint8_t EstablishedTiming2;								///<Byte36 Established Timings II
				std::uint8_t ManufacturerTiming;								///<Byte37 Manufacturer's Reserved Timings
			}EstablishedTimings;

			struct StandardTimingSection
			{
				std::uint16_t StandardTiming[8]; //Byte38-53 Standard timing information. Up to 8 2-byte fields describing standard display modes. Unused fields are filled with 01 01
			}StandardTimings;

			struct
			{
				union
				{
					EDID_Detailed_Timing_Descriptor DetailDesc;
					EDID_Descriptor_Common_String CommonDesc;
					EDID_Unspecified_Text Unspecified_Text;
					EDID_Display_Range_Limits_Descriptor Display_Range_Limits_Descriptor;
					EDID_Additional_White_Point_Descriptor Additional_White_Point_Descriptor;
					EDID_Color_Management_Data_Descriptor Color_Management_Data_Descriptor;
					EDID_CVT_Timing_Codes_Descriptor CVT_Timing_Codes_Descriptor;
					EDID_Additional_Standard_Timings Additional_Standard_Timings;
					EDID_StandardTimingIdentifierDefinition StandardTimingIdentifierDefinition;
				}Descriptor[4];
			}DataBlocks;

			std::uint8_t NumberOfExtensions;	//Byte126 Number of extensions to follow. 0 if no extensions.
			std::uint8_t Checksum;				//Byte127 Sum of all 128 bytes should equal 0 (mod 256).
		};

#pragma pack(pop)
	}
}