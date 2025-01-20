#ifndef CWELDINGGENERICCONTAINERPARAM_HPP
#define CWELDINGGENERICCONTAINERPARAM_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <gui_generated/containers/CWeldingGenericContainerParamBase.hpp>

class CWeldingGenericContainerParam : public CWeldingGenericContainerParamBase
{
public:
    CWeldingGenericContainerParam();
    virtual ~CWeldingGenericContainerParam() {}

    virtual void initialize();
	
	void set_TA_MainTitle(const TypedText &t);
	void setVisible_TA_MainTitle(bool b);
	void setActveBase(U8 i, bool b);

	void setAlphaContainer(U8);

	//@base 1
//       void set_TA_Main_1(const TypedText& t);
	void set_TA_Process_1(const TypedText& t);
	void set_TA_Title_1_Rx(const TypedText &t);
	void set_TA_Title_1_Sx(const char *t);
	void set_TA_Desc_1(const TypedText &t);
	void set_TA_Desc_1(const char *t);
	void set_IMG_iD_1(const Bitmap &v);
	void set_Color_BASE_1(colortype color);
	void setVisible_TA_Process_1(bool b);
	void setVisible_TA_Title_1_Rx(bool b);
	void setVisible_TA_Title_1_Sx(bool b);
	void setVisible_TA_Desc_1(bool b);
	void setVisible_IMG_iD_1(bool b);
	void setVisible_IMG_iTypo_1(bool b);
	//@base 2
	void setVisible_TA_Title_2_Rx(const TypedText &t);
	void set_TA_Desc_2(const TypedText &t);
	void set_TA_Desc_2(const char *t);
	void set_TA_Title_2_Rx(const TypedText& t);
	void set_IMG_iD_2(const Bitmap &v);
	void set_Color_BASE_2(colortype color);
	void setVisible_TA_Title_2_Rx(bool b);
	void setVisible_TA_Desc_2(bool b);
	void setVisible_IMG_iD_2(bool b);
	void setVisible_IMG_iTypo_2(bool b);
	//@base 3
//	void set_TA_Title_3(const TypedText &t);
//	void set_TA_Desc_3(const TypedText &t);
//	void set_TA_Desc_3(const char *t);
	void set_IMG_iD_3(const Bitmap &v);
	void set_Color_BASE_3(colortype color);
//	void setVisible_TA_Title_3(bool b);
//	void setVisible_TA_Desc_3(bool b);
	void setVisible_IMG_iD_3(bool b);
//	void setVisible_IMG_iTypo_3(bool b);



protected:
//	ColorDefined colorSelection;
	U8 activeBase[3];
};

#endif // CWELDINGGENERICCONTAINERPARAM_HPP
