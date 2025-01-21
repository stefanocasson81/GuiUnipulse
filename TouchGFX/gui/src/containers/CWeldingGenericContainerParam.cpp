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


void CWeldingGenericContainerParam::setBackground(colortype color)
{
   bBackground.setColor(color);
   bBackground.invalidate();
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


void CWeldingGenericContainerParam::setVisibleContainer(bool f)
{
   this->setVisible(f);
   this->invalidate();
}

//@base1
void CWeldingGenericContainerParam::set_TA_Process_1(const TypedText& t)
{
   tAProcess_1.setTypedText(t);
   tAProcess_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Process_1(bool b)
{
   tAProcess_1.setVisible(b);
   tAProcess_1.invalidate();
}



void CWeldingGenericContainerParam::set_TA_Title_1_Rx(const TypedText& t)
{
   tATitle_1_Rx.setTypedText(t);
   tATitle_1_Rx.invalidate();
}

void CWeldingGenericContainerParam::setVisible_TA_Title_1_Rx(bool b)
{
   tATitle_1_Rx.setVisible(b);
   tATitle_1_Rx.invalidate();
}



void CWeldingGenericContainerParam::set_TA_Title_1_Sx(const char *t)
{
   Unicode::strncpy(tATitle_1_SxBuffer,t, TADESC_1_SIZE-1);
   tATitle_1_Sx.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Title_1_Sx(bool b)
{
   tATitle_1_Sx.setVisible(b);
   tATitle_1_Sx.invalidate();
}



void CWeldingGenericContainerParam::set_TA_Desc_1(const char *t)
{
   Unicode::strncpy(tADesc_1Buffer,t, TADESC_1_SIZE-1);
   tADesc_1.invalidate();
}

void CWeldingGenericContainerParam::set_TA_Desc_1(const TypedText& t)
{
   tADesc_1.setTypedText(t);
   tADesc_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Desc_1(bool b)
{
   tADesc_1.setVisible(b);
   tADesc_1.invalidate();
}



void CWeldingGenericContainerParam::set_IMG_iD_1(const Bitmap& v)
{
   iD_1.setBitmap(v);
//   iD_1.setVisible(true);
   iD_1.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iD_1(bool b)
{
   iD_1.setVisible(b);
   iD_1.invalidate();
}



void CWeldingGenericContainerParam::set_Color_BASE_1(colortype color)
{
   bBase_1.setColor(color);
   bBase_1.invalidateContent();
}


void CWeldingGenericContainerParam::setVisible_IMG_iTypo_1(bool b)
{
   iTipo_1.setVisible(b);
   iTipo_1.invalidate();
}


/*********************base2**************************************/

void CWeldingGenericContainerParam::set_TA_Desc_2(const TypedText& t)
{
   tADesc_2.setTypedText(t);
   tADesc_2.invalidate();
}
void CWeldingGenericContainerParam::set_TA_Desc_2(const char *t)
{
   Unicode::strncpy(tADesc_2Buffer,t, TADESC_2_SIZE-1);
   tADesc_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Desc_2(bool b)
{
   tADesc_2.setVisible(b);
   tADesc_2.invalidate();
}



void CWeldingGenericContainerParam::set_TA_Title_2_Rx(const TypedText& t)
{
   tATitle_2_Rx.setTypedText(t);
   tATitle_2_Rx.invalidate();
}
void CWeldingGenericContainerParam::setVisible_TA_Title_2_Rx(bool b)
{
   tATitle_2_Rx.setVisible(b);
   tATitle_2_Rx.invalidate();
}



void CWeldingGenericContainerParam::set_IMG_iD_2(const Bitmap& v)
{
   iD_2.setBitmap(v);
//   iD_2.setVisible(true);
   iD_2.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iD_2(bool b)
{
   iD_2.setVisible(b);
   iD_2.invalidate();
}


void CWeldingGenericContainerParam::set_Color_BASE_2(colortype color)
{
   bBase_2.setColor(color);
   bBase_2.invalidate();
}

void CWeldingGenericContainerParam::setVisible_IMG_iTypo_2(bool b)
{
   iTipo_2.setVisible(b);
   iTipo_2.invalidate();
}

//@base3
void CWeldingGenericContainerParam::set_IMG_iD_3(const Bitmap& v)
{
   iD_3.setBitmap(v);
   iD_3.invalidate();
}
void CWeldingGenericContainerParam::setVisible_IMG_iD_3(bool b)
{
   iD_3.setVisible(b);
   iD_3.invalidate();
}


void CWeldingGenericContainerParam::setAlphaContainer(U8 v)
{
    bBackground.setAlpha(v);
    bBase_3.setAlpha(v);
    iD_3.setAlpha(v);
    bBase_2.setAlpha(v);
    iD_2.setAlpha(v);
    tATitle_2_Rx.setAlpha(v);
    tAProcess_2.setAlpha(v);
    tADesc_2.setAlpha(v);
    iTipo_2.setAlpha(v);
    bBase_1.setAlpha(v);
    iD_1.setAlpha(v);
    tATitle_1_Rx.setAlpha(v);
    tATitle_1_Sx.setAlpha(v);
    tAProcess_1.setAlpha(v);
    tADesc_1.setAlpha(v);
    iTipo_1.setAlpha(v);
    bMainTitle.setAlpha(v);
    tAMainTitle.setAlpha(v);
    this->invalidate();
}

