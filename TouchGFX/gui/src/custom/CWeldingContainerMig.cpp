#include <gui/custom/CWeldingContainerMig.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/custom/WeldingSupport.hpp>



CWeldingContainerMig::CWeldingContainerMig():indexMenu(0),indexSubMenu(0),maskDiameter(0)
{
   setContainerType(VIPERDEF_PROCESSO_MIG);
	CWeldingGenContParm[0].setXY(5,0);
	CWeldingGenContParm[1].setXY(165,0);
	CWeldingGenContParm[2].setXY(325,0);


	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		add(CWeldingGenContParm[i]);
	}

}


void CWeldingContainerMig::initialize()
{
   /***container 0***/
   CWeldingGenericContainer::initialize();
   CWeldingGenContParm[0].set_TA_Process_1(touchgfx::TypedText(T_PROC_MIG));

   CWeldingGenContParm[0].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

   CWeldingGenContParm[0].setVisible_TA_Title_1_Rx(false);

   CWeldingGenContParm[0].set_TA_Title_2_Rx(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[0].setVisible_TA_Title_2_Rx(true);

   CWeldingGenContParm[0].setVisible_TA_Desc_2(true);

   CWeldingGenContParm[0].set_IMG_iD_3(touchgfx::Bitmap(BITMAP_ICOMIG38X35_ID));
   CWeldingGenContParm[0].setVisible_IMG_iD_3(true);


   /***container 1***/
   CWeldingGenContParm[1].set_TA_MainTitle(touchgfx::TypedText(T_PROC_WIRE_TITLE));

   CWeldingGenContParm[1].set_TA_Title_1_Rx(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[1].setVisible_TA_Title_1_Rx(true);

   CWeldingGenContParm[1].setVisible_TA_Process_1(false);

   CWeldingGenContParm[1].set_TA_Title_2_Rx(touchgfx::TypedText(T_PROC_DIAMETER));
   CWeldingGenContParm[1].setVisible_TA_Title_2_Rx(true);

   CWeldingGenContParm[1].setVisible_TA_Desc_1(true);

   CWeldingGenContParm[1].setVisible_IMG_iD_2(false);

   //CWeldingGenContParm[1].set_TA_Desc_2(true);
   CWeldingGenContParm[1].setVisible_TA_Desc_2(true);

   CWeldingGenContParm[1].set_IMG_iD_3(touchgfx::Bitmap(BITMAP_ICOFILO30X35_ID));
   CWeldingGenContParm[1].setVisible_IMG_iD_3(true);



   /***container 2***/
   CWeldingGenContParm[2].set_TA_MainTitle(touchgfx::TypedText(T_PROC_GAS_TITLE));

   CWeldingGenContParm[2].setVisible_TA_Process_1(false);

   CWeldingGenContParm[2].set_TA_Title_1_Rx(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[2].setVisible_TA_Title_1_Rx(true);

//   CWeldingGenContParm[2].set_TA_Title_2_Rx(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[2].setVisible_TA_Title_2_Rx(false);

   CWeldingGenContParm[2].setVisible_TA_Desc_1(true);

  // CWeldingGenContParm[2].set_TA_Desc_2(true);
   CWeldingGenContParm[2].setVisible_TA_Desc_2(false);

   CWeldingGenContParm[2].set_IMG_iD_3(touchgfx::Bitmap(BITMAP_ICOGAS35X35_ID));
   CWeldingGenContParm[2].setVisible_IMG_iD_3(true);


   for (int i = 0; i < CWeldingGenContParm.size(); i++)
   {
      CWeldingGenContParm[i].initialize();
   }

   if ( initContainer_Callback && initContainer_Callback->isValid() )
   {
      initContainer_Callback->execute();
   }
}


void CWeldingContainerMig::confirmMenu(S8 p)
{

}
void CWeldingContainerMig::seletcMenu(S8 menu,S8 submenu)
{
	indexMenu = menu;
	indexSubMenu += submenu;
	for (int i = 0; i < CWeldingGenContParm.size(); i++)
	{
		CWeldingGenContParm[i].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
		CWeldingGenContParm[i].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
		CWeldingGenContParm[i].setAlphaContainer(90);
	}

	switch (indexMenu)
	{
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
         CWeldingGenContParm[0].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         CWeldingGenContParm[0].setAlphaContainer(255);
//         seletcMig_MigType(indexSubMenu);
      break;

      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
         CWeldingGenContParm[1].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         CWeldingGenContParm[1].setAlphaContainer(255);
//         seletcMig_WireType(indexSubMenu);
      break;
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
         CWeldingGenContParm[1].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         CWeldingGenContParm[1].setAlphaContainer(255);
//         seletcMig_WireDIameter(indexSubMenu);

      break;
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
         CWeldingGenContParm[2].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         CWeldingGenContParm[2].setAlphaContainer(255);
//         seletcMig_GasType(indexSubMenu);
      break;

      default:
         if ( indexMenu < VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG )
            indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
         else if ( indexMenu > VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS )
            indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
   }
   if ( menuSelected_Callback && menuSelected_Callback->isValid() )
   {
      menuSelected_Callback->execute(indexMenu,VIPERDEF_PROCESSO_MIG);
   }
}





void CWeldingContainerMig::seletcMig_MigType(S8 submenu)
{
   switch (submenu)
   {
      default:
         CWeldingGenContParm[0].set_TA_Desc_2("XxX");
      break;

      case VIPERDEF_TIPOMIG_MAN:
         CWeldingGenContParm[0].set_TA_Desc_2("MANUAL");
      break;
      case VIPERDEF_TIPOMIG_SYN:
         CWeldingGenContParm[0].set_TA_Desc_2("SYNERGIC");
      break;
      case VIPERDEF_TIPOMIG_PULSE:
         CWeldingGenContParm[0].set_TA_Desc_2("PULSED");
      break;
   }
}

void CWeldingContainerMig::seletcMig_WireDIameter(S8 submenu)
{
   switch (submenu)
   {
      default:
         CWeldingGenContParm[1].set_TA_Desc_2("XxX mm");
      break;
      case VIPERDEF_DIAMETROFILO_06:
         CWeldingGenContParm[1].set_TA_Desc_2("0.6 mm");
      break;
      case VIPERDEF_DIAMETROFILO_08:
         CWeldingGenContParm[1].set_TA_Desc_2("0.8 mm");
      break;
      case VIPERDEF_DIAMETROFILO_09:
         CWeldingGenContParm[1].set_TA_Desc_2("0.9 mm");
      break;
      case VIPERDEF_DIAMETROFILO_10:
         CWeldingGenContParm[1].set_TA_Desc_2("1.0 mm");
      break;
      case VIPERDEF_DIAMETROFILO_12:
         CWeldingGenContParm[1].set_TA_Desc_2("1.2 mm");
      break;
      case VIPERDEF_DIAMETROFILO_14:
         CWeldingGenContParm[1].set_TA_Desc_2("1.4 mm");
      break;
      case VIPERDEF_DIAMETROFILO_16:
         CWeldingGenContParm[1].set_TA_Desc_2("1.6 mm");
      break;
   }
}

void CWeldingContainerMig::seletcMig_WireType(S8 submenu)
{
   maskDiameter = 0;
   switch (submenu)
   {
      default:
         CWeldingGenContParm[1].set_TA_Desc_1("XxX");
      break;

      case VIPERDEF_TIPOFILO_MANUAL:
         CWeldingGenContParm[1].set_TA_Desc_1("MANUAL");
         maskDiameter = 1;
      break;
      case VIPERDEF_TIPOFILO_FE:
         CWeldingGenContParm[1].set_TA_Desc_1("Fe");
      break;
      case VIPERDEF_TIPOFILO_FEMETAL:
         CWeldingGenContParm[1].set_TA_Desc_1("FeMetal");
      break;
      case VIPERDEF_TIPOFILO_FERUTIL:
         CWeldingGenContParm[1].set_TA_Desc_1("FeRutil");
      break;
      case VIPERDEF_TIPOFILO_CRNI308:
         CWeldingGenContParm[1].set_TA_Desc_1("CrNi-308");
      break;
      case VIPERDEF_TIPOFILO_CRNI316:
         CWeldingGenContParm[1].set_TA_Desc_1("CrNi-316");
      break;
      case VIPERDEF_TIPOFILO_CRNI2209:
         CWeldingGenContParm[1].set_TA_Desc_1("CrNi-2209");
      break;
      case VIPERDEF_TIPOFILO_ALSI4043:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-4043");
      break;
      case VIPERDEF_TIPOFILO_AL4047:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-4047");
      break;
      case VIPERDEF_TIPOFILO_AL4145:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-4145");
      break;
      case VIPERDEF_TIPOFILO_AL4557:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-4557");
      break;
      case VIPERDEF_TIPOFILO_ALMG5356:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-5356");
      break;
      case VIPERDEF_TIPOFILO_AL5554:
         CWeldingGenContParm[1].set_TA_Desc_1("Al-5356");
      break;
      case VIPERDEF_TIPOFILO_CUSI3CRNI:
         CWeldingGenContParm[1].set_TA_Desc_1("CuSi3CrNi");
      break;
      case VIPERDEF_TIPOFILO_CUSI3ZN:
         CWeldingGenContParm[1].set_TA_Desc_1("CuSi3Zn");
      break;
      case VIPERDEF_TIPOFILO_CUAL8CRNI:
         CWeldingGenContParm[1].set_TA_Desc_1("CuAl8CrNi");
      break;
      case VIPERDEF_TIPOFILO_CUAL8ZN:
         CWeldingGenContParm[1].set_TA_Desc_1("CuAl8Zn");
      break;
      case VIPERDEF_TIPOFILO_ER80:
         CWeldingGenContParm[1].set_TA_Desc_1("Er80");
      break;
      case VIPERDEF_TIPOFILO_ER120:
         CWeldingGenContParm[1].set_TA_Desc_1("Er120");
      break;
      case VIPERDEF_TIPOFILO_NICR3:
         CWeldingGenContParm[1].set_TA_Desc_1("NiCr3");
      break;
   }

   if ( maskDiameter )
   {
      CWeldingGenContParm[1].setVisible_TA_Desc_2(false);
      CWeldingGenContParm[1].setVisible_IMG_iTypo_2(false);
      CWeldingGenContParm[1].setVisible_TA_Title_2_Rx(false);
   }
   else
   {
      CWeldingGenContParm[1].setVisible_TA_Desc_2(true);
      CWeldingGenContParm[1].setVisible_IMG_iTypo_2(true);
      CWeldingGenContParm[1].setVisible_TA_Title_2_Rx(true);
   }

}

void CWeldingContainerMig::seletcMig_GasType(S8 submenu)
{
   switch (submenu)
   {
      default:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("XxX");
         CWeldingGenContParm[2].set_TA_Desc_1("XxX");
      break;

      case VIPERDEF_TIPOGAS_NOGAS:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx(" ");
         CWeldingGenContParm[2].set_TA_Desc_1("No GAS");
      break;
      case VIPERDEF_TIPOGAS_I1_ARGON:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("I1");
         CWeldingGenContParm[2].set_TA_Desc_1("Argon");
      break;
      case VIPERDEF_TIPOGAS_M12_MIX9802:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("M12");
         CWeldingGenContParm[2].set_TA_Desc_1("Mix9802");
      break;
      case VIPERDEF_TIPOGAS_M20_MIX9208:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("M20");
         CWeldingGenContParm[2].set_TA_Desc_1("Mix9208");
      break;
      case VIPERDEF_TIPOGAS_M21_MIX8020:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("M21");
         CWeldingGenContParm[2].set_TA_Desc_1("Mix8020");
      break;
      case VIPERDEF_TIPOGAS_C1_CO2:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("C1");
         CWeldingGenContParm[2].set_TA_Desc_1("CO2");
      break;
      case VIPERDEF_TIPOGAS_C2_MIX9802:
         CWeldingGenContParm[2].setVisible_TA_Title_1_Rx("C2");
         CWeldingGenContParm[2].set_TA_Desc_1("Mix9802");
      break;

   }
}

