#ifndef CMAINMIG_HPP
#define CMAINMIG_HPP

#include <gui_generated/containers/CMainMigBase.hpp>


typedef enum
{
	CMAINMIG_SELEZIONATO_VOID,
	CMAINMIG_SELEZIONATO_EDITSX,
	CMAINMIG_SELEZIONATO_EDITDX,
	CMAINMIG_SELEZIONATO_WHATVISSX,
	CMAINMIG_SELEZIONATO_WHATVISDX,
	CMAINMIG_SELEZIONATO_IMPOSTAZIONERAPIDA,

	CMAINMIG_SELEZIONATO_MAX
}cmainmig_Selezionato_e;


class CMainMig : public CMainMigBase
{
public:
    CMainMig();
    virtual ~CMainMig() {}

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
    cmainmig_Selezionato_e Selezionato;
	cmainmig_Selezionato_e OldSelezionato;

	int32_t ValShowSX;
	int32_t ValShowDX;
	viperdef_MigSynSXVis_e MigSynSXVisShow;
	viperdef_MigSynDXVis_e MigSynDXVisShow;

	int32_t ValShowSX1Val;
	int32_t ValShowSX2Val;
	int32_t ValShowSX3Val;
	int32_t ValShowDX1Val;
	int32_t ValShowDX3Val;

	int16_t PosYWhatDataOriginal;
	int16_t PosYValDataOriginal;

	bool FlgRichiestoImpostazioneRapida;

	uint8_t IndiceImpostazioneRapida;
	uint8_t OldIndiceImpostazioneRapida;

	uint8_t BeepSegnalato;
};

#endif // CMAINMIG_HPP
