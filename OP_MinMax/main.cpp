#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"

/*	As of 0020, ExtractArgsEx() and ExtractFormatStringArgs() are no longer directly included in plugin builds.
	They are available instead through the OBSEScriptInterface.
	To make it easier to update plugins to account for this, the following can be used.
	It requires that g_scriptInterface is assigned correctly when the plugin is first loaded.
*/
#define ENABLE_EXTRACT_ARGS_MACROS 1	// #define this as 0 if you prefer not to use this

#if ENABLE_EXTRACT_ARGS_MACROS

OBSEScriptInterface * g_scriptInterface = NULL;	// make sure you assign to this
#define ExtractArgsEx(...) g_scriptInterface->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptInterface->ExtractFormatStringArgs(__VA_ARGS__)

#endif

#else
#include "obse_editor/EditorAPI.h"
#endif

#include "obse/ParamInfos.h"
#include "obse/Script.h"
#include "obse/GameObjects.h"
#include <string>

IDebugLog		gLog("OP_MinMax.log");

// Min
bool Cmd_Min_Execute(COMMAND_ARGS)
{
	// Declare
	UINT32	int1;
	UINT32	int2;
	// Extract My Arguments
	if(!ExtractArgs(PASS_EXTRACT_ARGS,&int1,&int2)) { _MESSAGE("Cmd_Min`CouldNotExtractArgs"); }
	// Perform Logic, Assign Result
	if(int1<int2)
		*result = int1;
	else
		*result = int2;
	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(Min,"Returns the min of two ints",0,2,kParams_TwoInts);
// Max
bool Cmd_Max_Execute(COMMAND_ARGS)
{
	// Declare
	UINT32	int1;
	UINT32	int2;
	// Extract My Arguments
	if(!ExtractArgs(PASS_EXTRACT_ARGS,&int1,&int2)) { _MESSAGE("Cmd_Min`CouldNotExtractArgs"); }
	// Perform Logic, Assign Result
	if(int1>int2)
		*result = int1;
	else
		*result = int2;
	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(Max,"Returns the max of two ints",0,2,kParams_TwoInts)

extern "C" {

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	_MESSAGE("query");
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "OP_MinMax";
	info->version = 1;
	// version checks
	if(!obse->isEditor)
	{
		_MESSAGE("OBSE version: %08X",obse->obseVersion);
		#if OBLIVION
		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
		#endif	
	}
	return true;
}

bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	_MESSAGE("load");
	obse->SetOpcodeBase(0x2000);
	obse->RegisterCommand(&kCommandInfo_Min);
	obse->RegisterCommand(&kCommandInfo_Max);
	if(!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptInterface = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
	}
	return true;
}

};
