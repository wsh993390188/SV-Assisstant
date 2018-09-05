#include "MonitorWidget.h"
#include <QListView>
#include <QScrollBar>
#include "lib\Hardware\SV_Hardware.h"

MonitorWidget::MonitorWidget(QWidget *parent)
	: QWidget(parent), EDIDTitle(new QComboBox(this)), EDIDData(new QTextBrowser(this)), mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void MonitorWidget::Init()
{
	EDIDTitle->setStyleSheet("QComboBox{border:1px solid gray;font-family: \"consolas\"; font-size:12px;}"
		"QComboBox QAbstractItemView::item{height:20px;}" //下拉选项高度
		"QComboBox::down-arrow:on{top:1px;left:1px;}"/* shift the arrow when popup is open */
		"QComboBox::drop-down{border:0px;}"); //下拉按钮
	EDIDTitle->setView(new QListView(EDIDTitle));
	auto edid = SV_ASSIST::Display::GetEDID();
	for (size_t i = 0; i < edid.size(); i++)
	{
		auto name = SV_ASSIST::Display::GetMonitorName(i);
		EDIDTitle->addItem(QString::fromStdString(name), QString::fromStdString(name));
	}
	EDIDTitle->setCurrentIndex(0);
	EDIDData->verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical{ background:#E4E4E4; padding:0px; border-radius:6px; max-width:12px;}"
		"QScrollBar::handle:vertical{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:vertical:hover{	background:#00BB9E; }"
		"QScrollBar::handle:vertical:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"		
		"QScrollBar::add-line:vertical{background:none;}"
		"QScrollBar::sub-line:vertical{background:none;}"
		"QScrollArea{border:0px; }");
	EDIDData->setStyleSheet("QTextBrowser{background-color: white;\
		border:none;color:#000000;font-family: \"consolas\"; font-size:13px;\
		background-attachment: scroll;}");
	UpdateEDID();
	connect(this->EDIDTitle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MonitorWidget::UpdateEDID);
	mainlayout->addWidget(EDIDTitle);
	mainlayout->addWidget(EDIDData);
}

void MonitorWidget::UpdateEDID()
{
	auto nums = EDIDTitle->currentIndex();
	auto edid = SV_ASSIST::Display::GetEDID().at(nums);
	EDIDManufacturer  buf = {};
	buf.__Manufacturer = edid.Manufacturer.Manufacturer;
	buf.__Manufacturer = ((buf.__Manufacturer & 0xFF) << 8) | ((buf.__Manufacturer & 0xFF00) >> 8);
	char temp[4];

	temp[0] = buf.bits._First + 'A' - 1;
	temp[1] = buf.bits._Second + 'A' - 1;
	temp[2] = buf.bits._Third + 'A' - 1;
	temp[3] = 0;
	EDIDData->append(QString(tr("		EDID ( Extended Display Identification Data) Report")));
	EDIDData->append(QString(tr("Vendor / Product Identification")));
	EDIDData->append(QString(tr("	Manufacture:                            %1").arg(temp)));
	EDIDData->append(QString(tr("	Monitor Serial Number :                 %1").arg(QString::number(edid.SerialNumber, 16)).toUpper()));
	EDIDData->append(QString(tr("	Product Code:                           %1").arg(QString::number(edid.Productcode, 16)).toUpper()));
	EDIDData->append(QString(tr("	Week/Year:                              %1").arg(edid.Week) + tr("/") + QString::number(edid.Year + 1990)));
	EDIDData->append(QString(tr("	Version:                                %1").arg(edid.EDIDVersion[0]) + tr(".") + QString::number(edid.EDIDVersion[1])));
	EDIDData->append(QString(tr("Video input parameters bitmap")));
	if (edid.VideoInputParameters & 0x80)
	{
		EDIDData->append(QString(tr("	Digital input")));
		short bitdepth = (edid.VideoInputParameters & 0x70) >> 4;
		if (bitdepth == 0 || bitdepth == 0x111)
			EDIDData->append(QString(tr("		Bit depth:                              Undefined")));
		else
			EDIDData->append(QString(tr("		Bit depth:                              %1").arg((bitdepth - 1) * 2 + 6)));
		short videointerface = edid.VideoInputParameters & 0xF;
		switch (videointerface)
		{
		case 1:
			EDIDData->append(QString(tr("		Video interface:                        HDMIa")));
			break;
		case 2:
			EDIDData->append(QString(tr("		Video interface:                        HDMIb")));
			break;
		case 4:
			EDIDData->append(QString(tr("		Video interface:                        MDDI")));
			break;
		case 5:
			EDIDData->append(QString(tr("		Video interface:                        DisplayPort")));
			break;
		default:
			EDIDData->append(QString(tr("		Video interface:                        Undefined")));
			break;
		}
	}
	else
	{
		EDIDData->append(QString(tr("	Analog input")));

	}
	EDIDData->append(QString(tr("		Max Horizontal Image Size:              %1 cm").arg(edid.HorizontalScreenSize)));
	EDIDData->append(QString(tr("		Max Vertical Image Size:                %1 cm").arg(edid.VerticalScreenSize)));
	EDIDData->append(QString(tr("		Gamma:                                  %1").arg(((edid.DisplayGamma + 100) / 100.0))));
	EDIDData->append(QString(tr("FeatureSupport:")));
	EDIDData->append(QString(tr("		Standby:                                ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("		Suspend:                                ") + (edid.Features & 0x40 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("		Active off:                             ") + (edid.Features & 0x20 ? tr("Yes") : tr("No"))));
	//    EDIDData->append(QString(tr("            standby:                                ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
	//    EDIDData->append(QString(tr("            Color space:                            ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("		standby:                                ") + (edid.Features & 0x04 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("		Preferred timing mode specified:        ") + (edid.Features & 0x02 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("		Continuous timings with GTF or CVT:     ") + (edid.Features & 0x01 ? tr("Yes") : tr("No"))));
	EDIDData->append(QString(tr("Color Characteristic:")));
	double Red_x = ((edid.RedxValue << 2) | ((edid.RedAndGreen & 0xC0) >> 6));
	double Red_y = ((edid.RedyValue << 2) | ((edid.RedAndGreen & 0x30) >> 4));
	double Green_x = ((edid.GreenxyValue[0] << 2) | ((edid.RedAndGreen & 0x0C) >> 2));
	double Green_y = ((edid.GreenxyValue[1] << 2) | ((edid.RedAndGreen & 0x03) >> 0));
	double Blue_x = ((edid.BluexyValue[0] << 2) | ((edid.BlueAndWhite & 0xC0) >> 6));
	double Blue_y = ((edid.BluexyValue[1] << 2) | ((edid.BlueAndWhite & 0x30) >> 4));
	double White_x = ((edid.WhitexyValue[0] << 2) | ((edid.BlueAndWhite & 0x0C) >> 2));
	double White_y = ((edid.WhitexyValue[1] << 2) | ((edid.BlueAndWhite & 0x03) >> 0));
	EDIDData->append(QString(tr("		Red:                              X = %1  Y = %2 ").arg(QString::number(Red_x / 1024, 'f', 3)).arg(QString::number(Red_y / 1024, 'f', 3))));
	EDIDData->append(QString(tr("		Green:                            X = %1  Y = %2 ").arg(QString::number(Green_x / 1024, 'f', 3)).arg(QString::number(Green_y / 1024, 'f', 3))));
	EDIDData->append(QString(tr("		Blue:                             X = %1  Y = %2 ").arg(QString::number(Blue_x / 1024, 'f', 3)).arg(QString::number(Blue_y / 1024, 'f', 3))));
	EDIDData->append(QString(tr("		White:                            X = %1  Y = %2 ").arg(QString::number(White_x / 1024, 'f', 3)).arg(QString::number(White_y / 1024, 'f', 3))));
	//...
	EDIDData->append(QString(tr("Established Timings:")));
	if (edid.EstablishedTiming[0] & 0x80)
		EDIDData->append(QString(tr("		720x400 @ 70 Hz ")));
	if (edid.EstablishedTiming[0] & 0x40)
		EDIDData->append(QString(tr("		720x400 @ 88 Hz ")));
	if (edid.EstablishedTiming[0] & 0x20)
		EDIDData->append(QString(tr("		640x480 @ 60 Hz ")));
	if (edid.EstablishedTiming[0] & 0x10)
		EDIDData->append(QString(tr("		640x480 @ 67 Hz ")));
	if (edid.EstablishedTiming[0] & 0x08)
		EDIDData->append(QString(tr("		640x480 @ 72 Hz ")));
	if (edid.EstablishedTiming[0] & 0x04)
		EDIDData->append(QString(tr("		640x480 @ 75 Hz ")));
	if (edid.EstablishedTiming[0] & 0x02)
		EDIDData->append(QString(tr("		800x600 @ 56 Hz ")));
	if (edid.EstablishedTiming[0] & 0x01)
		EDIDData->append(QString(tr("		800x600 @ 60 Hz ")));

	if (edid.EstablishedTiming[1] & 0x80)
		EDIDData->append(QString(tr("		800x600 @ 72 Hz ")));
	if (edid.EstablishedTiming[1] & 0x40)
		EDIDData->append(QString(tr("		800x600 @ 75 Hz ")));
	if (edid.EstablishedTiming[1] & 0x20)
		EDIDData->append(QString(tr("		832x624 @ 75 Hz ")));
	if (edid.EstablishedTiming[1] & 0x10)
		EDIDData->append(QString(tr("		1024x768 @ 87 Hz ")));
	if (edid.EstablishedTiming[1] & 0x08)
		EDIDData->append(QString(tr("		1024x768 @ 60 Hz ")));
	if (edid.EstablishedTiming[1] & 0x04)
		EDIDData->append(QString(tr("		1024x768 @ 72 Hz ")));
	if (edid.EstablishedTiming[1] & 0x02)
		EDIDData->append(QString(tr("		1024x768 @ 75 Hz ")));
	if (edid.EstablishedTiming[1] & 0x01)
		EDIDData->append(QString(tr("		1280x1024 @ 75 Hz ")));

	if (edid.EstablishedTiming[2] & 0x80)
		EDIDData->append(QString(tr("		1152x870 @ 75Hz ")));

	for (int i = 0; i < 16; i += 2)
	{
		if (edid.StandardTiming[i] == 0 || (edid.StandardTiming[i] == 1 && 1 == edid.StandardTiming[i + 1]))
		{
			EDIDData->append(QString(tr("Standard Timing #%1").arg(i / 2)));
			EDIDData->append(QString(tr("	Unused")));
		}
		else
		{
			EDIDData->append(QString(tr("Standard Timing #%1").arg(i / 2)));
			int horizontal = (edid.StandardTiming[i] + 31) * 8;
			int RefreshRate = (edid.StandardTiming[i + 1] & 0x3F) + 60;
			switch (((edid.StandardTiming[i] + 1) & 0xC) >> 6)
			{
			case 0:
				EDIDData->append(QString(tr("		%1 x %2 @ %3 Hz").arg(horizontal).arg(horizontal / 16 * 10).arg(RefreshRate)));
				break;
			case 1:
				EDIDData->append(QString(tr("		%1 x %1 @ %3 Hz").arg(horizontal).arg(horizontal / 4 * 3).arg(RefreshRate)));
				break;
			case 2:
				EDIDData->append(QString(tr("		%1 x %1 @ %3 Hz").arg(horizontal).arg(horizontal / 5 * 4).arg(RefreshRate)));
				break;
			case 4:
				EDIDData->append(QString(tr("		%1 x %2 @ %3 Hz").arg(horizontal).arg(horizontal / 16 * 9).arg(RefreshRate)));
				break;
			}

		}

	}
	for (short i = 0; i < 4; i++)
	{
		EDID_Other_Monitor_Descriptors OtherDesc;
		int minvoffset, minhoffset, maxvoffset, maxhoffset;
		memcpy(&OtherDesc, edid.Descriptor + i, sizeof(EDID_Other_Monitor_Descriptors));
		if (OtherDesc.Reserve == 0 && OtherDesc.Reserved[0] == 0 && OtherDesc.Reserved[1] == 0 && OtherDesc.Reserved[2] == 0)
		{
			QString t = QString::fromUtf8((const char*)edid.Descriptor[i].Display_Serial_Number.Display_Serial_Number, 13);
			switch (OtherDesc.Descriptor_Type)
			{
			case 0xFF:
				EDIDData->append(QString(tr("Display serial number Descriptor:")));
				EDIDData->append(QString(tr("	Display serial number:                          %1").arg(t.simplified())));
				break;
			case 0xFD:
				EDIDData->append(QString(tr("Display Range Limits Descriptor:")));
				edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x1 ? minvoffset = 0xFF : minvoffset = 0;
				edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x2 ? minhoffset = 0xFF : minhoffset = 0;
				edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x4 ? maxvoffset = 0xFF : maxvoffset = 0;
				edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x8 ? maxhoffset = 0xFF : maxhoffset = 0;
				EDIDData->append(QString(tr("	Min. Horizontal rate:                       %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_horizontal_field_rate + minhoffset)));
				EDIDData->append(QString(tr("	Max. Horizontal rate:                       %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_horizontal_field_rate + maxhoffset)));
				EDIDData->append(QString(tr("	Min. Vertical rate:                         %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_vertical_field_rate + minvoffset)));
				EDIDData->append(QString(tr("	Max. Vertical rate:                         %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_vertical_field_rate + maxvoffset)));
				EDIDData->append(QString(tr("	Max. Pixel Clock:                           %1 Mhz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_pixel_clock_rate * 10)));

				break;
			case 0xFC:
				EDIDData->append(QString(tr("Display name Descriptor:")));
				EDIDData->append(QString(tr("	Display name number:                          %1").arg(t.simplified())));
				break;
			default:
				EDIDData->append(QString(tr("Descriptors # %1").arg(i)));
				EDIDData->append(QString(tr("	To be continued")));
				break;
			}
		}
		else
		{
			EDIDData->append(QString(tr("Detailed Timing Descriptor:")));
			int temp = edid.Descriptor[i].DetailDesc.Pixel_Clock;
			//temp = ((temp & 0xFF) << 8) | ((temp & 0xFF00) >> 8);
			EDIDData->append(QString(tr("	Pixel Clock:            %1 Mhz").arg(temp / 100.0)));
			int Horizontala = edid.Descriptor[i].DetailDesc.Horizental_active_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizental_active_pixels << 8);
			int Horizontalb = edid.Descriptor[i].DetailDesc.Horizontal_blanking_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizontal_blanking_pixels << 8);
			EDIDData->append(QString(tr("	Horizontal active:          %1 Pixels").arg(Horizontala)));
			EDIDData->append(QString(tr("	Horizontal blanking:        %1 Pixels").arg(Horizontalb)));
			int Verticala = edid.Descriptor[i].DetailDesc.Vertical_active_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_active_pixels << 8);
			int Verticalb = edid.Descriptor[i].DetailDesc.Vertical_blanking_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_blanking_pixels << 8);
			EDIDData->append(QString(tr("	Vertical  active:           %1 Pixels").arg(Verticala)));
			EDIDData->append(QString(tr("	Vertical  blanking:         %1 Pixels").arg(Verticalb)));
			int Horizontaloffset = edid.Descriptor[i].DetailDesc.Horizontal_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_front_porch << 8);
			int Horizontalwidth = edid.Descriptor[i].DetailDesc.Horizontal_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_sync_pulse_width << 8);
			int Verticaloffset = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_front_porch << 8);
			int Verticalwidth = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_sync_pulse_width << 8);
			EDIDData->append(QString(tr("	Horizontal sync. offset:    %1 pixels").arg(Horizontaloffset)));
			EDIDData->append(QString(tr("	Horizontal sync. width:     %1 pixels").arg(Horizontalwidth)));
			EDIDData->append(QString(tr("	Vertical sync. offset:      %1 pixels").arg(Verticaloffset)));
			EDIDData->append(QString(tr("	Vertical sync. width:       %1 pixels").arg(Verticalwidth)));
			int Horizonimage = edid.Descriptor[i].DetailDesc.Horizontal_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Horizontal_image_size << 8);
			int verticalimage = edid.Descriptor[i].DetailDesc.Vertical_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Vertical_image_size << 8);
			EDIDData->append(QString(tr("	Horizontal Image Size:      %1 mm").arg(Horizonimage)));
			EDIDData->append(QString(tr("	Vertical Image Size:        %1 mm").arg(verticalimage)));

			EDIDData->append(QString(tr("	Horizontal Border:          %1 pixels").arg(edid.Descriptor[i].DetailDesc.Horizontal_border_pixels)));
			EDIDData->append(QString(tr("	Vertical Border:            %1 lines").arg(edid.Descriptor[i].DetailDesc.Vertical_border_lines)));
		}
	}
	EDIDData->moveCursor(QTextCursor::Start);
}

MonitorWidget::~MonitorWidget()
{
}
