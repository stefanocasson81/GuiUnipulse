#include <gui/containers/CWeldingGenericContainerParam.hpp>

CWeldingGenericContainerParam::CWeldingGenericContainerParam()
{

}

void CWeldingGenericContainerParam::initialize()
{
    CWeldingGenericContainerParamBase::initialize();
}

void CWeldingGenericContainerParam::setActveBase(U8 i,bool b)
{
   activeBase[i] = b;
}


void CWeldingGenericContainerParam::set_TA_MainTitle(const TypedText& t)
{
   tAMainTitle.setTypedText(t);
   tAMainTitle.invalidate();
}

//void CWeldingGenericContainerParam::set_TA_Main_1(const TypedText& t)
//{
//	tAMain_1.setTypedText(t);
//	tAMain_1.invalidate();
//}


void CWeldingGenericContainerParam::setVisible_TA_MainTitle(bool b)
{
   tAMainTitle.setVisible(b);
   tAMainTitle.invalidate();
}


//@base1
void CWeldingGenericContainerParam::set_TA_Process_1(const TypedText& t)
{
   tAProcess_1.setTypedText(t);
   tAProcess_1.invalidate();
}

void CWeldingGenericContainerParam::set_TA_Title_1(const TypedText& t)
{
   tATitle_1.setTypedText(t);
   tATitle_1.invalidate();
}
void CWeldingGenericContainerParam::set_TA_Desc_1(const char *t)
{
   Unicode::strncpy(tADesc_1Buffer,t, TADESC_1_SIZE-1);
}

void CWeldingGenericContainerParam::set_TA_Desc_1(const TypedText& t)
{
   tADesc_1.setTypedText(t);
   tADesc_1.invalidate();
}
void CWeldingGenericContainerParam::set_IMG_iD_1(const Bitmap& v)
{
   iD_1.setBitmap(v);
   iD_1.setVisible(true);
   iD_1.invalidate();
}

void CWeldingGenericContainerParam::set_Color_BASE_1(colortype color)
{
   bBase_1.setColor(color);
   bBase_1.invalidateContent();
}

void CWeldingGenericContainerParam::setVisible_TA_Process_1(bool b)
{
   tAProcess_1.setVisible(b);
   tAProcess_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Title_1(bool b)
{
   tATitle_1.setVisible(b);
   tATitle_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Desc_1(bool b)
{
   tADesc_1.setVisible(b);
   tADesc_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iD_1(bool b)
{
   iD_1.setVisible(b);
   iD_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iTypo_1(bool b)
{
   iTipo_1.setVisible(b);
   iTipo_1.invalidate();
}

//@base2
void CWeldingGenericContainerParam::set_TA_Title_2(const TypedText& t)
{
   tATitle_2.setTypedText(t);
   tATitle_2.invalidate();
}
void CWeldingGenericContainerParam::set_TA_Desc_2(const TypedText& t)
{
   tADesc_2.setTypedText(t);
   tADesc_2.invalidate();
}
void CWeldingGenericContainerParam::set_TA_Desc_2(const char *t)
{
   Unicode::strncpy(tADesc_2Buffer,t, TADESC_2_SIZE-1);
}
void CWeldingGenericContainerParam::set_IMG_iD_2(const Bitmap& v)
{
   iD_2.setBitmap(v);
   iD_2.setVisible(true);
   iD_2.invalidate();
}

void CWeldingGenericContainerParam::set_Color_BASE_2(colortype color)
{
   bBase_2.setColor(color);
   bBase_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Title_2(bool b)
{
   tATitle_2.setVisible(b);
   tATitle_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Desc_2(bool b)
{
   tADesc_2.setVisible(b);
   tADesc_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iD_2(bool b)
{
   iD_2.setVisible(b);
   iD_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iTypo_2(bool b)
{
   iTipo_2.setVisible(b);
   iTipo_2.invalidate();
}

//@base3


