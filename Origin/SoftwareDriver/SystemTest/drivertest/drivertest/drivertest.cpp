// drivertest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdlib.h>
#include <conio.h>
#include <winioctl.h>
#include <SetupAPI.h>
#include <initguid.h>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../../SystemTest/Public.h"
#include "../../SystemTest/ioctl.h"
#include "../../SystemTest/Driver/defination.h"

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")

using namespace std;

template<typename T>
void SafeFree(T*& x, DWORD &count)
{
	HeapFree(GetProcessHeap(), 0, x);
	x = nullptr;
	count = 0;
}
const WCHAR* const SHADOWSSDT_FUNC_NAME[] = 
{
	L"NtUserGetOwnerTransformedMonitorRect",
	L"NtUserYieldTask",
	L"NtUserSetSensorPresence",
	L"NtUserGetThreadState",
	L"NtUserPeekMessage",
	L"NtUserCallOneParam",
	L"NtUserGetKeyState",
	L"NtUserInvalidateRect								  ",
	L"NtUserCallNoParam",
	L"NtUserGetMessage",
	L"NtUserMessageCall",
	L"NtGdiBitBlt		",
	L"NtGdiGetCharSet	",
	L"NtUserGetDC		",
	L"NtGdiSelectBitmap",
	L"NtUserWaitMessage",
	L"NtUserTranslateMessage								  ",
	L"NtUserGetProp	",
	L"NtUserPostMessage",
	L"NtUserQueryWindow",
	L"NtUserTranslateAccelerator							  ",
	L"NtGdiFlush		",
	L"NtUserRedrawWindow",
	L"NtUserWindowFromPoint								  ",
	L"NtUserCallMsgFilter",
	L"NtUserValidateTimerCallback							  ",
	L"NtUserBeginPaint",
	L"NtUserSetTimer	",
	L"NtUserEndPaint	",
	L"NtUserSetCursor	",
	L"NtUserKillTimer	",
	L"NtUserBuildHwndList",
	L"NtUserSelectPalette",
	L"NtUserCallNextHookEx								  ",
	L"NtUserHideCaret	",
	L"NtGdiIntersectClipRect								  ",
	L"NtUserCallHwndLock",
	L"NtUserGetProcessWindowStation						  ",
	L"NtGdiDeleteObjectApp								  ",
	L"NtUserSetWindowPos",
	L"NtUserShowCaret	",
	L"NtUserEndDeferWindowPosEx							  ",
	L"NtUserCallHwndParamLock								  ",
	L"NtUserVkKeyScanEx",
	L"NtGdiSetDIBitsToDeviceInternal						  ",
	L"NtUserCallTwoParam",
	L"NtGdiGetRandomRgn",
	L"NtUserCopyAcceleratorTable							  ",
	L"NtUserNotifyWinEvent								  ",
	L"NtGdiExtSelectClipRgn								  ",
	L"NtUserIsClipboardFormatAvailable					  ",
	L"NtUserSetScrollInfo",
	L"NtGdiStretchBlt	",
	L"NtUserCreateCaret",
	L"NtGdiRectVisible",
	L"NtGdiCombineRgn	",
	L"NtGdiGetDCObject",
	L"NtUserDispatchMessage								  ",
	L"NtUserRegisterWindowMessage							  ",
	L"NtGdiExtTextOutW",
	L"NtGdiSelectFont	",
	L"NtGdiRestoreDC	",
	L"NtGdiSaveDC		",
	L"NtUserGetForegroundWindow							  ",
	L"NtUserShowScrollBar",
	L"NtUserFindExistingCursorIcon						  ",
	L"NtGdiGetDCDword	",
	L"NtGdiGetRegionData",
	L"NtGdiLineTo		",
	L"NtUserSystemParametersInfo							  ",
	L"NtGdiGetAppClipBox",
	L"NtUserGetAsyncKeyState								  ",
	L"NtUserGetCPD	",
	L"NtUserRemoveProp",
	L"NtGdiDoPalette	",
	L"NtGdiPolyPolyDraw",
	L"NtUserSetCapture",
	L"NtUserEnumDisplayMonitors							  ",
	L"NtGdiCreateCompatibleBitmap							  ",
	L"NtUserSetProp	",
	L"NtGdiGetTextCharsetInfo								  ",
	L"NtUserSBGetParms",
	L"NtUserGetIconInfo",
	L"NtUserExcludeUpdateRgn								  ",
	L"NtUserSetFocus	",
	L"NtGdiExtGetObjectW",
	L"NtUserGetUpdateRect",
	L"NtGdiCreateCompatibleDC								  ",
	L"NtUserGetClipboardSequenceNumber					  ",
	L"NtGdiCreatePen	",
	L"NtUserShowWindow",
	L"NtUserGetKeyboardLayoutList							  ",
	L"NtGdiPatBlt		",
	L"NtUserMapVirtualKeyEx								  ",
	L"NtUserSetWindowLong",
	L"NtGdiHfontCreate",
	L"NtUserMoveWindow",
	L"NtUserPostThreadMessage								  ",
	L"NtUserDrawIconEx",
	L"NtUserGetSystemMenu",
	L"NtGdiDrawStream	",
	L"NtUserInternalGetWindowText							  ",
	L"NtUserGetWindowDC",
	L"NtGdiInvertRgn	",
	L"NtGdiGetRgnBox	",
	L"NtGdiGetAndSetDCDword								  ",
	L"NtGdiMaskBlt	",
	L"NtGdiGetWidthTable",
	L"NtUserScrollDC	",
	L"NtUserGetObjectInformation							  ",
	L"NtGdiCreateBitmap",
	L"NtUserFindWindowEx",
	L"NtGdiPolyPatBlt	",
	L"NtUserUnhookWindowsHookEx							  ",
	L"NtGdiGetNearestColor								  ",
	L"NtGdiTransformPoints								  ",
	L"NtGdiGetDCPoint	",
	L"NtGdiCreateDIBBrush",
	L"NtGdiGetTextMetricsW								  ",
	L"NtUserCreateWindowEx								  ",
	L"NtUserSetParent	",
	L"NtUserGetKeyboardState								  ",
	L"NtUserToUnicodeEx",
	L"NtUserGetControlBrush								  ",
	L"NtUserGetClassName",
	L"NtGdiAlphaBlend	",
	L"NtGdiOffsetRgn	",
	L"NtUserDefSetText",
	L"NtGdiGetTextFaceW",
	L"NtGdiStretchDIBitsInternal							  ",
	L"NtUserSendInput	",
	L"NtUserGetThreadDesktop								  ",
	L"NtGdiCreateRectRgn",
	L"NtGdiGetDIBitsInternal								  ",
	L"NtUserGetUpdateRgn",
	L"NtGdiDeleteClientObj								  ",
	L"NtUserGetIconSize",
	L"NtUserFillWindow",
	L"NtGdiExtCreateRegion								  ",
	L"NtGdiComputeXformCoefficients						  ",
	L"NtUserSetWindowsHookEx								  ",
	L"NtUserNotifyProcessCreate							  ",
	L"NtGdiUnrealizeObject								  ",
	L"NtUserGetTitleBarInfo								  ",
	L"NtGdiRectangle	",
	L"NtUserSetThreadDesktop								  ",
	L"NtUserGetDCEx	",
	L"NtUserGetScrollBarInfo								  ",
	L"NtGdiGetTextExtent",
	L"NtUserSetWindowFNID",
	L"NtGdiSetLayout	",
	L"NtUserCalcMenuBar",
	L"NtUserThunkedMenuItemInfo							  ",
	L"NtGdiExcludeClipRect								  ",
	L"NtGdiCreateDIBSection								  ",
	L"NtGdiGetDCforBitmap",
	L"NtUserDestroyCursor",
	L"NtUserDestroyWindow",
	L"NtUserCallHwndParam",
	L"NtGdiCreateDIBitmapInternal							  ",
	L"NtUserOpenWindowStation								  ",
	L"NtUserSetCursorIconData								  ",
	L"NtUserCloseDesktop",
	L"NtUserOpenDesktop",
	L"NtUserSetProcessWindowStation						  ",
	L"NtUserGetAtomName",
	L"NtGdiExtCreatePen",
	L"NtGdiCreatePaletteInternal							  ",
	L"NtGdiSetBrushOrg",
	L"NtUserBuildNameList",
	L"NtGdiSetPixel	",
	L"NtUserRegisterClassExWOW							  ",
	L"NtGdiCreatePatternBrushInternal						  ",
	L"NtUserGetAncestor",
	L"NtGdiGetOutlineTextMetricsInternalW					  ",
	L"NtGdiSetBitmapBits",
	L"NtUserCloseWindowStation							  ",
	L"NtUserGetDoubleClickTime							  ",
	L"NtUserEnableScrollBar								  ",
	L"NtGdiCreateSolidBrush								  ",
	L"NtUserGetClassInfoEx								  ",
	L"NtGdiCreateClientObj								  ",
	L"NtUserUnregisterClass								  ",
	L"NtUserDeleteMenu",
	L"NtGdiRectInRegion",
	L"NtUserScrollWindowEx								  ",
	L"NtGdiGetPixel	",
	L"NtUserSetClassLong",
	L"NtUserGetMenuBarInfo								  ",
	L"NtGdiGetNearestPaletteIndex							  ",
	L"NtGdiGetCharWidthW",
	L"NtUserInvalidateRgn",
	L"NtUserGetClipboardOwner								  ",
	L"NtUserSetWindowRgn",
	L"NtUserBitBltSysBmp",
	L"NtGdiGetCharWidthInfo								  ",
	L"NtUserValidateRect",
	L"NtUserCloseClipboard								  ",
	L"NtUserOpenClipboard",
	L"NtUserSetClipboardData								  ",
	L"NtUserEnableMenuItem								  ",
	L"NtUserAlterWindowStyle								  ",
	L"NtGdiFillRgn	",
	L"NtUserGetWindowPlacement							  ",
	L"NtGdiModifyWorldTransform							  ",
	L"NtGdiGetFontData",
	L"NtUserGetOpenClipboardWindow						  ",
	L"NtUserSetThreadState								  ",
	L"NtGdiOpenDCW	",
	L"NtUserTrackMouseEvent								  ",
	L"NtGdiGetTransform",
	L"NtUserDestroyMenu",
	L"NtGdiGetBitmapBits",
	L"NtUserConsoleControl								  ",
	L"NtUserSetActiveWindow								  ",
	L"NtUserSetInformationThread							  ",
	L"NtUserSetWindowPlacement							  ",
	L"NtUserGetControlColor								  ",
	L"NtGdiSetMetaRgn	",
	L"NtGdiSetMiterLimit",
	L"NtGdiSetVirtualResolution							  ",
	L"NtGdiGetRasterizerCaps								  ",
	L"NtUserSetWindowWord",
	L"NtUserGetClipboardFormatName						  ",
	L"NtUserRealInternalGetMessage						  ",
	L"NtUserCreateLocalMemHandle							  ",
	L"NtUserAttachThreadInput								  ",
	L"NtGdiCreateHalftonePalette							  ",
	L"NtUserPaintMenuBar",
	L"NtUserSetKeyboardState								  ",
	L"NtGdiCombineTransform								  ",
	L"NtUserCreateAcceleratorTable						  ",
	L"NtUserGetCursorFrameInfo							  ",
	L"NtUserGetAltTabInfo",
	L"NtUserGetCaretBlinkTime								  ",
	L"NtGdiQueryFontAssocInfo								  ",
	L"NtUserProcessConnect								  ",
	L"NtUserEnumDisplayDevices							  ",
	L"NtUserEmptyClipboard								  ",
	L"NtUserGetClipboardData								  ",
	L"NtUserRemoveMenu",
	L"NtGdiSetBoundsRect",
	L"NtGdiGetBitmapDimension								  ",
	L"NtUserConvertMemHandle								  ",
	L"NtUserDestroyAcceleratorTable						  ",
	L"NtUserGetGUIThreadInfo								  ",
	L"NtGdiCloseFigure",
	L"NtUserSetWindowsHookAW								  ",
	L"NtUserSetMenuDefaultItem							  ",
	L"NtUserCheckMenuItem",
	L"NtUserSetWinEventHook								  ",
	L"NtUserUnhookWinEvent								  ",
	L"NtUserLockWindowUpdate								  ",
	L"NtUserSetSystemMenu",
	L"NtUserThunkedMenuInfo								  ",
	L"NtGdiBeginPath	",
	L"NtGdiEndPath	",
	L"NtGdiFillPath	",
	L"NtUserCallHwnd	",
	L"NtUserDdeInitialize",
	L"NtUserModifyUserStartupInfoFlags					  ",
	L"NtUserCountClipboardFormats							  ",
	L"NtGdiAddFontMemResourceEx							  ",
	L"NtGdiEqualRgn	",
	L"NtGdiGetSystemPaletteUse							  ",
	L"NtGdiRemoveFontMemResourceEx						  ",
	L"NtUserEnumDisplaySettings							  ",
	L"NtUserPaintDesktop",
	L"NtGdiExtEscape	",
	L"NtGdiSetBitmapDimension								  ",
	L"NtGdiSetFontEnumeration								  ",
	L"NtUserChangeClipboardChain							  ",
	L"NtUserSetClipboardViewer							  ",
	L"NtUserShowWindowAsync								  ",
	L"NtGdiCreateColorSpace								  ",
	L"NtGdiDeleteColorSpace								  ",
	L"NtUserActivateKeyboardLayout						  ",
	L"NtBindCompositionSurface							  ",
	L"NtCompositionInputThread							  ",
	L"NtCompositionSetDropTarget							  ",
	L"NtCreateCompositionInputSink						  ",
	L"NtCreateCompositionSurfaceHandle					  ",
	L"NtCreateImplicitCompositionInputSink				  ",
	L"NtDCompositionAddCrossDeviceVisualChild				  ",
	L"NtDCompositionBeginFrame							  ",
	L"NtDCompositionCommitChannel							  ",
	L"NtDCompositionCommitSynchronizationObject			  ",
	L"NtDCompositionConfirmFrame							  ",
	L"NtDCompositionConnectPipe							  ",
	L"NtDCompositionCreateAndBindSharedSection			  ",
	L"NtDCompositionCreateChannel							  ",
	L"NtDCompositionCreateConnection						  ",
	L"NtDCompositionCreateDwmChannel						  ",
	L"NtDCompositionCreateSharedVisualHandle				  ",
	L"NtDCompositionCurrentBatchId						  ",
	L"NtDCompositionDestroyChannel						  ",
	L"NtDCompositionDestroyConnection						  ",
	L"NtDCompositionDiscardFrame							  ",
	L"NtDCompositionDuplicateHandleToProcess				  ",
	L"NtDCompositionDuplicateSwapchainHandleToDwm			  ",
	L"NtDCompositionEnableDDASupport						  ",
	L"NtDCompositionEnableMMCSS							  ",
	L"NtDCompositionGetChannels							  ",
	L"NtDCompositionGetConnectionBatch					  ",
	L"NtDCompositionGetDeletedResources					  ",
	L"NtDCompositionGetFrameLegacyTokens					  ",
	L"NtDCompositionGetFrameStatistics					  ",
	L"NtDCompositionGetFrameSurfaceUpdates				  ",
	L"NtDCompositionProcessChannelBatchBuffer				  ",
	L"NtDCompositionReferenceSharedResourceOnDwmChannel	  ",
	L"NtDCompositionRegisterThumbnailVisual				  ",
	L"NtDCompositionRegisterVirtualDesktopVisual			  ",
	L"NtDCompositionReleaseAllResources					  ",
	L"NtDCompositionRemoveCrossDeviceVisualChild			  ",
	L"NtDCompositionRetireFrame							  ",
	L"NtDCompositionSetChannelCallbackId					  ",
	L"NtDCompositionSetChannelCommitCompletionEvent		  ",
	L"NtDCompositionSetChildRootVisual					  ",
	L"NtDCompositionSetDebugCounter						  ",
	L"NtDCompositionSubmitDWMBatch						  ",
	L"NtDCompositionSynchronize							  ",
	L"NtDCompositionTelemetryAnimationScenarioBegin		  ",
	L"NtDCompositionTelemetryAnimationScenarioReference	  ",
	L"NtDCompositionTelemetryAnimationScenarioUnreference	  ",
	L"NtDCompositionTelemetrySetApplicationId				  ",
	L"NtDCompositionTelemetryTouchInteractionBegin		  ",
	L"NtDCompositionTelemetryTouchInteractionEnd			  ",
	L"NtDCompositionTelemetryTouchInteractionUpdate		  ",
	L"NtDCompositionUpdatePointerCapture					  ",
	L"NtDCompositionWaitForChannel						  ",
	L"NtDWMBindCursorToOutputConfig						  ",
	L"NtDWMCommitInputSystemOutputConfig					  ",
	L"NtDWMSetCursorOrientation							  ",
	L"NtDWMSetInputSystemOutputConfig						  ",
	L"NtDesktopCaptureBits								  ",
	L"NtDuplicateCompositionInputSink						  ",
	L"NtFlipObjectAddPoolBuffer							  ",
	L"NtFlipObjectCreate",
	L"NtFlipObjectOpen",
	L"NtFlipObjectRemovePoolBuffer						  ",
	L"NtGdiAbortDoc	",
	L"NtGdiAbortPath	",
	L"NtGdiAddEmbFontToDC",
	L"NtGdiAddFontResourceW								  ",
	L"NtGdiAddInitialFonts								  ",
	L"NtGdiAddRemoteFontToDC								  ",
	L"NtGdiAddRemoteMMInstanceToDC						  ",
	L"NtGdiAngleArc	",
	L"NtGdiAnyLinkedFonts",
	L"NtGdiArcInternal",
	L"NtGdiBRUSHOBJ_DeleteRbrush							  ",
	L"NtGdiBRUSHOBJ_hGetColorTransform					  ",
	L"NtGdiBRUSHOBJ_pvAllocRbrush							  ",
	L"NtGdiBRUSHOBJ_pvGetRbrush							  ",
	L"NtGdiBRUSHOBJ_ulGetBrushColor						  ",
	L"NtGdiBeginGdiRendering								  ",
	L"NtGdiCLIPOBJ_bEnum",
	L"NtGdiCLIPOBJ_cEnumStart								  ",
	L"NtGdiCLIPOBJ_ppoGetPath								  ",
	L"NtGdiCancelDC	",
	L"NtGdiChangeGhostFont								  ",
	L"NtGdiCheckBitmapBits								  ",
	L"NtGdiClearBitmapAttributes							  ",
	L"NtGdiClearBrushAttributes							  ",
	L"NtGdiColorCorrectPalette							  ",
	L"NtGdiConfigureOPMProtectedOutput					  ",
	L"NtGdiConvertMetafileRect							  ",
	L"NtGdiCreateBitmapFromDxSurface						  ",
	L"NtGdiCreateBitmapFromDxSurface2						  ",
	L"NtGdiCreateColorTransform							  ",
	L"NtGdiCreateEllipticRgn								  ",
	L"NtGdiCreateHatchBrushInternal						  ",
	L"NtGdiCreateMetafileDC								  ",
	L"NtGdiCreateOPMProtectedOutput						  ",
	L"NtGdiCreateOPMProtectedOutputs						  ",
	L"NtGdiCreateRoundRectRgn								  ",
	L"NtGdiCreateServerMetaFile							  ",
	L"NtGdiCreateSessionMappedDIBSection					  ",
	L"NtGdiDDCCIGetCapabilitiesString						  ",
	L"NtGdiDDCCIGetCapabilitiesStringLength				  ",
	L"NtGdiDDCCIGetTimingReport							  ",
	L"NtGdiDDCCIGetVCPFeature								  ",
	L"NtGdiDDCCISaveCurrentSettings						  ",
	L"NtGdiDDCCISetVCPFeature								  ",
	L"NtGdiDdCreateFullscreenSprite						  ",
	L"NtGdiDdDDIAbandonSwapChain							  ",
	L"NtGdiDdDDIAcquireKeyedMutex							  ",
	L"NtGdiDdDDIAcquireKeyedMutex2						  ",
	L"NtGdiDdDDIAcquireSwapChain							  ",
	L"NtGdiDdDDIAddSurfaceToSwapChain						  ",
	L"NtGdiDdDDIAdjustFullscreenGamma						  ",
	L"NtGdiDdDDICacheHybridQueryValue						  ",
	L"NtGdiDdDDIChangeVideoMemoryReservation				  ",
	L"NtGdiDdDDICheckExclusiveOwnership					  ",
	L"NtGdiDdDDICheckMonitorPowerState					  ",
	L"NtGdiDdDDICheckMultiPlaneOverlaySupport				  ",
	L"NtGdiDdDDICheckMultiPlaneOverlaySupport2			  ",
	L"NtGdiDdDDICheckMultiPlaneOverlaySupport3			  ",
	L"NtGdiDdDDICheckOcclusion							  ",
	L"NtGdiDdDDICheckSharedResourceAccess					  ",
	L"NtGdiDdDDICheckVidPnExclusiveOwnership				  ",
	L"NtGdiDdDDICloseAdapter								  ",
	L"NtGdiDdDDIConfigureSharedResource					  ",
	L"NtGdiDdDDICreateAllocation							  ",
	L"NtGdiDdDDICreateBundleObject						  ",
	L"NtGdiDdDDICreateContext								  ",
	L"NtGdiDdDDICreateContextVirtual						  ",
	L"NtGdiDdDDICreateDCFromMemory						  ",
	L"NtGdiDdDDICreateDevice								  ",
	L"NtGdiDdDDICreateHwContext							  ",
	L"NtGdiDdDDICreateHwQueue								  ",
	L"NtGdiDdDDICreateKeyedMutex							  ",
	L"NtGdiDdDDICreateKeyedMutex2							  ",
	L"NtGdiDdDDICreateOutputDupl							  ",
	L"NtGdiDdDDICreateOverlay								  ",
	L"NtGdiDdDDICreatePagingQueue							  ",
	L"NtGdiDdDDICreateProtectedSession					  ",
	L"NtGdiDdDDICreateSwapChain							  ",
	L"NtGdiDdDDICreateSynchronizationObject				  ",
	L"NtGdiDdDDIDDisplayEnum								  ",
	L"NtGdiDdDDIDestroyAllocation							  ",
	L"NtGdiDdDDIDestroyAllocation2						  ",
	L"NtGdiDdDDIDestroyContext							  ",
	L"NtGdiDdDDIDestroyDCFromMemory						  ",
	L"NtGdiDdDDIDestroyDevice								  ",
	L"NtGdiDdDDIDestroyHwContext							  ",
	L"NtGdiDdDDIDestroyHwQueue							  ",
	L"NtGdiDdDDIDestroyKeyedMutex							  ",
	L"NtGdiDdDDIDestroyOutputDupl							  ",
	L"NtGdiDdDDIDestroyOverlay							  ",
	L"NtGdiDdDDIDestroyPagingQueue						  ",
	L"NtGdiDdDDIDestroyProtectedSession					  ",
	L"NtGdiDdDDIDestroySynchronizationObject				  ",
	L"NtGdiDdDDIDispMgrCreate								  ",
	L"NtGdiDdDDIDispMgrSourceOperation					  ",
	L"NtGdiDdDDIDispMgrTargetOperation					  ",
	L"NtGdiDdDDIEnumAdapters								  ",
	L"NtGdiDdDDIEnumAdapters2								  ",
	L"NtGdiDdDDIEscape",
	L"NtGdiDdDDIEvict	",
	L"NtGdiDdDDIExtractBundleObject						  ",
	L"NtGdiDdDDIFlipOverlay								  ",
	L"NtGdiDdDDIFlushHeapTransitions						  ",
	L"NtGdiDdDDIFreeGpuVirtualAddress						  ",
	L"NtGdiDdDDIGetAllocationPriority						  ",
	L"NtGdiDdDDIGetCachedHybridQueryValue					  ",
	L"NtGdiDdDDIGetContextInProcessSchedulingPriority		  ",
	L"NtGdiDdDDIGetContextSchedulingPriority				  ",
	L"NtGdiDdDDIGetDWMVerticalBlankEvent					  ",
	L"NtGdiDdDDIGetDeviceState							  ",
	L"NtGdiDdDDIGetDisplayModeList						  ",
	L"NtGdiDdDDIGetMemoryBudgetTarget						  ",
	L"NtGdiDdDDIGetMultiPlaneOverlayCaps					  ",
	L"NtGdiDdDDIGetMultisampleMethodList					  ",
	L"NtGdiDdDDIGetOverlayState							  ",
	L"NtGdiDdDDIGetPostCompositionCaps					  ",
	L"NtGdiDdDDIGetPresentHistory							  ",
	L"NtGdiDdDDIGetPresentQueueEvent						  ",
	L"NtGdiDdDDIGetProcessDeviceLostSupport				  ",
	L"NtGdiDdDDIGetProcessSchedulingPriorityBand			  ",
	L"NtGdiDdDDIGetProcessSchedulingPriorityClass			  ",
	L"NtGdiDdDDIGetResourcePresentPrivateDriverData		  ",
	L"NtGdiDdDDIGetRuntimeData							  ",
	L"NtGdiDdDDIGetScanLine								  ",
	L"NtGdiDdDDIGetSetSwapChainMetadata					  ",
	L"NtGdiDdDDIGetSharedPrimaryHandle					  ",
	L"NtGdiDdDDIGetSharedResourceAdapterLuid				  ",
	L"NtGdiDdDDIGetYieldPercentage						  ",
	L"NtGdiDdDDIInvalidateActiveVidPn						  ",
	L"NtGdiDdDDIInvalidateCache							  ",
	L"NtGdiDdDDILock	",
	L"NtGdiDdDDILock2	",
	L"NtGdiDdDDIMakeResident								  ",
	L"NtGdiDdDDIMapGpuVirtualAddress						  ",
	L"NtGdiDdDDIMarkDeviceAsError							  ",
	L"NtGdiDdDDINetDispGetNextChunkInfo					  ",
	L"NtGdiDdDDINetDispQueryMiracastDisplayDeviceStatus	  ",
	L"NtGdiDdDDINetDispQueryMiracastDisplayDeviceSupport	  ",
	L"NtGdiDdDDINetDispStartMiracastDisplayDevice			  ",
	L"NtGdiDdDDINetDispStopMiracastDisplayDevice			  ",
	L"NtGdiDdDDINetDispStopSessions						  ",
	L"NtGdiDdDDIOfferAllocations							  ",
	L"NtGdiDdDDIOpenAdapterFromDeviceName					  ",
	L"NtGdiDdDDIOpenAdapterFromHdc						  ",
	L"NtGdiDdDDIOpenAdapterFromLuid						  ",
	L"NtGdiDdDDIOpenKeyedMutex							  ",
	L"NtGdiDdDDIOpenKeyedMutex2							  ",
	L"NtGdiDdDDIOpenNtHandleFromName						  ",
	L"NtGdiDdDDIOpenProtectedSessionFromNtHandle			  ",
	L"NtGdiDdDDIOpenResource								  ",
	L"NtGdiDdDDIOpenResourceFromNtHandle					  ",
	L"NtGdiDdDDIOpenSwapChain								  ",
	L"NtGdiDdDDIOpenSyncObjectFromNtHandle				  ",
	L"NtGdiDdDDIOpenSyncObjectFromNtHandle2				  ",
	L"NtGdiDdDDIOpenSyncObjectNtHandleFromName			  ",
	L"NtGdiDdDDIOpenSynchronizationObject					  ",
	L"NtGdiDdDDIOutputDuplGetFrameInfo					  ",
	L"NtGdiDdDDIOutputDuplGetMetaData						  ",
	L"NtGdiDdDDIOutputDuplGetPointerShapeData				  ",
	L"NtGdiDdDDIOutputDuplPresent							  ",
	L"NtGdiDdDDIOutputDuplReleaseFrame					  ",
	L"NtGdiDdDDIPinDirectFlipResources					  ",
	L"NtGdiDdDDIPollDisplayChildren						  ",
	L"NtGdiDdDDIPresent",
	L"NtGdiDdDDIPresentMultiPlaneOverlay					  ",
	L"NtGdiDdDDIPresentMultiPlaneOverlay2					  ",
	L"NtGdiDdDDIPresentMultiPlaneOverlay3					  ",
	L"NtGdiDdDDIPresentRedirected							  ",
	L"NtGdiDdDDIQueryAdapterInfo							  ",
	L"NtGdiDdDDIQueryAllocationResidency					  ",
	L"NtGdiDdDDIQueryClockCalibration						  ",
	L"NtGdiDdDDIQueryFSEBlock								  ",
	L"NtGdiDdDDIQueryProcessOfferInfo						  ",
	L"NtGdiDdDDIQueryProtectedSessionInfoFromNtHandle		  ",
	L"NtGdiDdDDIQueryProtectedSessionStatus				  ",
	L"NtGdiDdDDIQueryRemoteVidPnSourceFromGdiDisplayName	  ",
	L"NtGdiDdDDIQueryResourceInfo							  ",
	L"NtGdiDdDDIQueryResourceInfoFromNtHandle				  ",
	L"NtGdiDdDDIQueryStatistics							  ",
	L"NtGdiDdDDIQueryVidPnExclusiveOwnership				  ",
	L"NtGdiDdDDIQueryVideoMemoryInfo						  ",
	L"NtGdiDdDDIReclaimAllocations						  ",
	L"NtGdiDdDDIReclaimAllocations2						  ",
	L"NtGdiDdDDIReleaseKeyedMutex							  ",
	L"NtGdiDdDDIReleaseKeyedMutex2						  ",
	L"NtGdiDdDDIReleaseProcessVidPnSourceOwners			  ",
	L"NtGdiDdDDIReleaseSwapChain							  ",
	L"NtGdiDdDDIRemoveSurfaceFromSwapChain				  ",
	L"NtGdiDdDDIRender",
	L"NtGdiDdDDIReserveGpuVirtualAddress					  ",
	L"NtGdiDdDDISetAllocationPriority						  ",
	L"NtGdiDdDDISetContextInProcessSchedulingPriority		  ",
	L"NtGdiDdDDISetContextSchedulingPriority				  ",
	L"NtGdiDdDDISetDeviceLostSupport						  ",
	L"NtGdiDdDDISetDisplayMode							  ",
	L"NtGdiDdDDISetDisplayPrivateDriverFormat				  ",
	L"NtGdiDdDDISetDodIndirectSwapchain					  ",
	L"NtGdiDdDDISetFSEBlock								  ",
	L"NtGdiDdDDISetGammaRamp								  ",
	L"NtGdiDdDDISetHwProtectionTeardownRecovery			  ",
	L"NtGdiDdDDISetMemoryBudgetTarget						  ",
	L"NtGdiDdDDISetMonitorColorSpaceTransform				  ",
	L"NtGdiDdDDISetProcessSchedulingPriorityBand			  ",
	L"NtGdiDdDDISetProcessSchedulingPriorityClass			  ",
	L"NtGdiDdDDISetQueuedLimit							  ",
	L"NtGdiDdDDISetStablePowerState						  ",
	L"NtGdiDdDDISetStereoEnabled							  ",
	L"NtGdiDdDDISetSyncRefreshCountWaitTarget				  ",
	L"NtGdiDdDDISetVidPnSourceHwProtection				  ",
	L"NtGdiDdDDISetVidPnSourceOwner						  ",
	L"NtGdiDdDDISetYieldPercentage						  ",
	L"NtGdiDdDDIShareObjects								  ",
	L"NtGdiDdDDISharedPrimaryLockNotification				  ",
	L"NtGdiDdDDISharedPrimaryUnLockNotification			  ",
	L"NtGdiDdDDISignalSynchronizationObject				  ",
	L"NtGdiDdDDISignalSynchronizationObjectFromCpu		  ",
	L"NtGdiDdDDISignalSynchronizationObjectFromGpu		  ",
	L"NtGdiDdDDISignalSynchronizationObjectFromGpu2		  ",
	L"NtGdiDdDDISubmitCommand								  ",
	L"NtGdiDdDDISubmitCommandToHwQueue					  ",
	L"NtGdiDdDDISubmitSignalSyncObjectsToHwQueue			  ",
	L"NtGdiDdDDISubmitWaitForSyncObjectsToHwQueue			  ",
	L"NtGdiDdDDITrimProcessCommitment						  ",
	L"NtGdiDdDDIUnOrderedPresentSwapChain					  ",
	L"NtGdiDdDDIUnlock",
	L"NtGdiDdDDIUnlock2",
	L"NtGdiDdDDIUnpinDirectFlipResources					  ",
	L"NtGdiDdDDIUpdateAllocationProperty					  ",
	L"NtGdiDdDDIUpdateGpuVirtualAddress					  ",
	L"NtGdiDdDDIUpdateOverlay								  ",
	L"NtGdiDdDDIWaitForIdle								  ",
	L"NtGdiDdDDIWaitForSynchronizationObject				  ",
	L"NtGdiDdDDIWaitForSynchronizationObjectFromCpu		  ",
	L"NtGdiDdDDIWaitForSynchronizationObjectFromGpu		  ",
	L"NtGdiDdDDIWaitForVerticalBlankEvent					  ",
	L"NtGdiDdDDIWaitForVerticalBlankEvent2				  ",
	L"NtGdiDdDestroyFullscreenSprite						  ",
	L"NtGdiDdNotifyFullscreenSpriteUpdate					  ",
	L"NtGdiDdQueryVisRgnUniqueness						  ",
	L"NtGdiDeleteColorTransform							  ",
	L"NtGdiDescribePixelFormat							  ",
	L"NtGdiDestroyOPMProtectedOutput						  ",
	L"NtGdiDestroyPhysicalMonitor							  ",
	L"NtGdiDoBanding	",
	L"NtGdiDrawEscape	",
	L"NtGdiDwmCreatedBitmapRemotingOutput					  ",
	L"NtGdiEllipse	",
	L"NtGdiEnableEudc	",
	L"NtGdiEndDoc		",
	L"NtGdiEndGdiRendering								  ",
	L"NtGdiEndPage	",
	L"NtGdiEngAlphaBlend",
	L"NtGdiEngAssociateSurface							  ",
	L"NtGdiEngBitBlt	",
	L"NtGdiEngCheckAbort",
	L"NtGdiEngComputeGlyphSet								  ",
	L"NtGdiEngCopyBits",
	L"NtGdiEngCreateBitmap								  ",
	L"NtGdiEngCreateClip",
	L"NtGdiEngCreateDeviceBitmap							  ",
	L"NtGdiEngCreateDeviceSurface							  ",
	L"NtGdiEngCreatePalette								  ",
	L"NtGdiEngDeleteClip",
	L"NtGdiEngDeletePalette								  ",
	L"NtGdiEngDeletePath",
	L"NtGdiEngDeleteSurface								  ",
	L"NtGdiEngEraseSurface								  ",
	L"NtGdiEngFillPath",
	L"NtGdiEngGradientFill								  ",
	L"NtGdiEngLineTo	",
	L"NtGdiEngLockSurface",
	L"NtGdiEngMarkBandingSurface							  ",
	L"NtGdiEngPaint	",
	L"NtGdiEngPlgBlt	",
	L"NtGdiEngStretchBlt",
	L"NtGdiEngStretchBltROP								  ",
	L"NtGdiEngStrokeAndFillPath							  ",
	L"NtGdiEngStrokePath",
	L"NtGdiEngTextOut	",
	L"NtGdiEngTransparentBlt								  ",
	L"NtGdiEngUnlockSurface								  ",
	L"NtGdiEnsureDpiDepDefaultGuiFontForPlateau			  ",
	L"NtGdiEnumFonts	",
	L"NtGdiEnumObjects",
	L"NtGdiEudcLoadUnloadLink								  ",
	L"NtGdiExtFloodFill",
	L"NtGdiFONTOBJ_cGetAllGlyphHandles					  ",
	L"NtGdiFONTOBJ_cGetGlyphs								  ",
	L"NtGdiFONTOBJ_pQueryGlyphAttrs						  ",
	L"NtGdiFONTOBJ_pfdg",
	L"NtGdiFONTOBJ_pifi",
	L"NtGdiFONTOBJ_pvTrueTypeFontFile						  ",
	L"NtGdiFONTOBJ_pxoGetXform							  ",
	L"NtGdiFONTOBJ_vGetInfo								  ",
	L"NtGdiFlattenPath",
	L"NtGdiFontIsLinked",
	L"NtGdiForceUFIMapping								  ",
	L"NtGdiFrameRgn	",
	L"NtGdiFullscreenControl								  ",
	L"NtGdiGetAppliedDeviceGammaRamp						  ",
	L"NtGdiGetBitmapDpiScaleValue							  ",
	L"NtGdiGetBoundsRect",
	L"NtGdiGetCOPPCompatibleOPMInformation				  ",
	L"NtGdiGetCertificate",
	L"NtGdiGetCertificateByHandle							  ",
	L"NtGdiGetCertificateSize								  ",
	L"NtGdiGetCertificateSizeByHandle						  ",
	L"NtGdiGetCharABCWidthsW								  ",
	L"NtGdiGetCharacterPlacementW							  ",
	L"NtGdiGetColorAdjustment								  ",
	L"NtGdiGetColorSpaceforBitmap							  ",
	L"NtGdiGetCurrentDpiInfo								  ",
	L"NtGdiGetDCDpiScaleValue								  ",
	L"NtGdiGetDeviceCaps",
	L"NtGdiGetDeviceCapsAll								  ",
	L"NtGdiGetDeviceGammaRamp								  ",
	L"NtGdiGetDeviceWidth",
	L"NtGdiGetDhpdev	",
	L"NtGdiGetETM		",
	L"NtGdiGetEmbUFI	",
	L"NtGdiGetEmbedFonts",
	L"NtGdiGetEntry	",
	L"NtGdiGetEudcTimeStampEx								  ",
	L"NtGdiGetFontFileData								  ",
	L"NtGdiGetFontFileInfo								  ",
	L"NtGdiGetFontResourceInfoInternalW					  ",
	L"NtGdiGetFontUnicodeRanges							  ",
	L"NtGdiGetGammaRampCapability							  ",
	L"NtGdiGetGlyphIndicesW								  ",
	L"NtGdiGetGlyphIndicesWInternal						  ",
	L"NtGdiGetGlyphOutline								  ",
	L"NtGdiGetKerningPairs								  ",
	L"NtGdiGetLinkedUFIs",
	L"NtGdiGetMiterLimit",
	L"NtGdiGetMonitorID",
	L"NtGdiGetNumberOfPhysicalMonitors					  ",
	L"NtGdiGetOPMInformation								  ",
	L"NtGdiGetOPMRandomNumber								  ",
	L"NtGdiGetObjectBitmapHandle							  ",
	L"NtGdiGetPath	",
	L"NtGdiGetPerBandInfo",
	L"NtGdiGetPhysicalMonitorDescription					  ",
	L"NtGdiGetPhysicalMonitors							  ",
	L"NtGdiGetProcessSessionFonts							  ",
	L"NtGdiGetPublicFontTableChangeCookie					  ",
	L"NtGdiGetRealizationInfo								  ",
	L"NtGdiGetServerMetaFileBits							  ",
	L"NtGdiGetSpoolMessage								  ",
	L"NtGdiGetStats	",
	L"NtGdiGetStringBitmapW								  ",
	L"NtGdiGetSuggestedOPMProtectedOutputArraySize		  ",
	L"NtGdiGetTextExtentExW								  ",
	L"NtGdiGetUFI		",
	L"NtGdiGetUFIPathname",
	L"NtGdiGradientFill",
	L"NtGdiHLSurfGetInformation							  ",
	L"NtGdiHLSurfSetInformation							  ",
	L"NtGdiHT_Get8BPPFormatPalette						  ",
	L"NtGdiHT_Get8BPPMaskPalette							  ",
	L"NtGdiIcmBrushInfo",
	L"NtGdiInit		",
	L"NtGdiInitSpool	",
	L"NtGdiMakeFontDir",
	L"NtGdiMakeInfoDC	",
	L"NtGdiMakeObjectUnXferable							  ",
	L"NtGdiMakeObjectXferable								  ",
	L"NtGdiMirrorWindowOrg								  ",
	L"NtGdiMonoBitmap	",
	L"NtGdiMoveTo		",
	L"NtGdiOffsetClipRgn",
	L"NtGdiPATHOBJ_bEnum",
	L"NtGdiPATHOBJ_bEnumClipLines							  ",
	L"NtGdiPATHOBJ_vEnumStart								  ",
	L"NtGdiPATHOBJ_vEnumStartClipLines					  ",
	L"NtGdiPATHOBJ_vGetBounds								  ",
	L"NtGdiPathToRegion",
	L"NtGdiPlgBlt		",
	L"NtGdiPolyDraw	",
	L"NtGdiPolyTextOutW",
	L"NtGdiPtInRegion	",
	L"NtGdiPtVisible	",
	L"NtGdiQueryFonts	",
	L"NtGdiRemoveFontResourceW							  ",
	L"NtGdiRemoveMergeFont								  ",
	L"NtGdiResetDC	",
	L"NtGdiResizePalette",
	L"NtGdiRoundRect	",
	L"NtGdiSTROBJ_bEnum",
	L"NtGdiSTROBJ_bEnumPositionsOnly						  ",
	L"NtGdiSTROBJ_bGetAdvanceWidths						  ",
	L"NtGdiSTROBJ_dwGetCodePage							  ",
	L"NtGdiSTROBJ_vEnumStart								  ",
	L"NtGdiScaleRgn	",
	L"NtGdiScaleValues",
	L"NtGdiScaleViewportExtEx								  ",
	L"NtGdiScaleWindowExtEx								  ",
	L"NtGdiSelectBrush",
	L"NtGdiSelectClipPath",
	L"NtGdiSelectPen	",
	L"NtGdiSetBitmapAttributes							  ",
	L"NtGdiSetBrushAttributes								  ",
	L"NtGdiSetColorAdjustment								  ",
	L"NtGdiSetColorSpace",
	L"NtGdiSetDeviceGammaRamp								  ",
	L"NtGdiSetFontXform",
	L"NtGdiSetIcmMode	",
	L"NtGdiSetLinkedUFIs",
	L"NtGdiSetMagicColors",
	L"NtGdiSetOPMSigningKeyAndSequenceNumbers				  ",
	L"NtGdiSetPUMPDOBJ",
	L"NtGdiSetPixelFormat",
	L"NtGdiSetPrivateDeviceGammaRamp						  ",
	L"NtGdiSetRectRgn	",
	L"NtGdiSetSizeDevice",
	L"NtGdiSetSystemPaletteUse							  ",
	L"NtGdiSetTextJustification							  ",
	L"NtGdiSetUMPDSandboxState							  ",
	L"NtGdiStartDoc	",
	L"NtGdiStartPage	",
	L"NtGdiStrokeAndFillPath								  ",
	L"NtGdiStrokePath	",
	L"NtGdiSwapBuffers",
	L"NtGdiTransparentBlt",
	L"NtGdiUMPDEngFreeUserMem								  ",
	L"NtGdiUnloadPrinterDriver							  ",
	L"NtGdiUnmapMemFont",
	L"NtGdiUpdateColors",
	L"NtGdiUpdateTransform								  ",
	L"NtGdiWidenPath	",
	L"NtGdiXFORMOBJ_bApplyXform							  ",
	L"NtGdiXFORMOBJ_iGetXform								  ",
	L"NtGdiXLATEOBJ_cGetPalette							  ",
	L"NtGdiXLATEOBJ_hGetColorTransform					  ",
	L"NtGdiXLATEOBJ_iXlate								  ",
	L"NtHWCursorUpdatePointer								  ",
	L"NtMITActivateInputProcessing						  ",
	L"NtMITBindInputTypeToMonitors						  ",
	L"NtMITCoreMsgKGetConnectionHandle					  ",
	L"NtMITCoreMsgKOpenConnectionTo						  ",
	L"NtMITCoreMsgKSend",
	L"NtMITDeactivateInputProcessing						  ",
	L"NtMITDisableMouseIntercept							  ",
	L"NtMITEnableMouseIntercept							  ",
	L"NtMITGetCursorUpdateHandle							  ",
	L"NtMITSetInputCallbacks								  ",
	L"NtMITSynthesizeMouseInput							  ",
	L"NtMITSynthesizeMouseWheel							  ",
	L"NtMITSynthesizeTouchInput							  ",
	L"NtMITUpdateInputGlobals								  ",
	L"NtMITWaitForMultipleObjectsEx						  ",
	L"NtNotifyPresentToCompositionSurface					  ",
	L"NtOpenCompositionSurfaceDirtyRegion					  ",
	L"NtOpenCompositionSurfaceSectionInfo					  ",
	L"NtOpenCompositionSurfaceSwapChainHandleInfo			  ",
	L"NtQueryCompositionInputIsImplicit					  ",
	L"NtQueryCompositionInputQueueAndTransform			  ",
	L"NtQueryCompositionInputSink							  ",
	L"NtQueryCompositionInputSinkLuid						  ",
	L"NtQueryCompositionInputSinkViewId					  ",
	L"NtQueryCompositionSurfaceBinding					  ",
	L"NtQueryCompositionSurfaceHDRMetaData				  ",
	L"NtQueryCompositionSurfaceRenderingRealization		  ",
	L"NtQueryCompositionSurfaceStatistics					  ",
	L"NtRIMAddInputObserver								  ",
	L"NtRIMAreSiblingDevices								  ",
	L"NtRIMDeviceIoControl								  ",
	L"NtRIMEnableMonitorMappingForDevice					  ",
	L"NtRIMFreeInputBuffer								  ",
	L"NtRIMGetDevicePreparsedData							  ",
	L"NtRIMGetDevicePreparsedDataLockfree					  ",
	L"NtRIMGetDeviceProperties							  ",
	L"NtRIMGetDevicePropertiesLockfree					  ",
	L"NtRIMGetPhysicalDeviceRect							  ",
	L"NtRIMGetSourceProcessId								  ",
	L"NtRIMObserveNextInput								  ",
	L"NtRIMOnPnpNotification								  ",
	L"NtRIMOnTimerNotification							  ",
	L"NtRIMReadInput	",
	L"NtRIMRegisterForInput								  ",
	L"NtRIMRemoveInputObserver							  ",
	L"NtRIMSetTestModeStatus								  ",
	L"NtRIMUnregisterForInput								  ",
	L"NtRIMUpdateInputObserverRegistration				  ",
	L"NtSetCompositionSurfaceAnalogExclusive				  ",
	L"NtSetCompositionSurfaceBufferUsage					  ",
	L"NtSetCompositionSurfaceDirectFlipState				  ",
	L"NtSetCompositionSurfaceHDRMetaData					  ",
	L"NtSetCompositionSurfaceIndependentFlipInfo			  ",
	L"NtSetCompositionSurfaceStatistics					  ",
	L"NtTokenManagerConfirmOutstandingAnalogToken			  ",
	L"NtTokenManagerCreateCompositionTokenHandle			  ",
	L"NtTokenManagerGetAnalogExclusiveSurfaceUpdates		  ",
	L"NtTokenManagerGetAnalogExclusiveTokenEvent			  ",
	L"NtTokenManagerOpenSectionAndEvents					  ",
	L"NtTokenManagerThread								  ",
	L"NtUnBindCompositionSurface							  ",
	L"NtUpdateInputSinkTransforms							  ",
	L"NtUserAcquireIAMKey",
	L"NtUserAcquireInteractiveControlBackgroundAccess		  ",
	L"NtUserAddClipboardFormatListener					  ",
	L"NtUserAssociateInputContext							  ",
	L"NtUserAutoPromoteMouseInPointer						  ",
	L"NtUserAutoRotateScreen								  ",
	L"NtUserBeginLayoutUpdate								  ",
	L"NtUserBlockInput",
	L"NtUserBroadcastThemeChangeEvent						  ",
	L"NtUserBuildHimcList",
	L"NtUserBuildPropList",
	L"NtUserCalculatePopupWindowPosition					  ",
	L"NtUserCallHwndOpt",
	L"NtUserCanBrokerForceForeground						  ",
	L"NtUserChangeDisplaySettings							  ",
	L"NtUserChangeWindowMessageFilterEx					  ",
	L"NtUserCheckAccessForIntegrityLevel					  ",
	L"NtUserCheckProcessForClipboardAccess				  ",
	L"NtUserCheckProcessSession							  ",
	L"NtUserCheckWindowThreadDesktop						  ",
	L"NtUserChildWindowFromPointEx						  ",
	L"NtUserClearForeground								  ",
	L"NtUserClipCursor",
	L"NtUserCompositionInputSinkLuidFromPoint				  ",
	L"NtUserCompositionInputSinkViewInstanceIdFromPoint	  ",
	L"NtUserConfirmResizeCommit							  ",
	L"NtUserCreateDCompositionHwndTarget					  ",
	L"NtUserCreateDesktopEx								  ",
	L"NtUserCreateEmptyCursorObject						  ",
	L"NtUserCreateInputContext							  ",
	L"NtUserCreateWindowStation							  ",
	L"NtUserCtxDisplayIOCtl								  ",
	L"NtUserDeferWindowPosAndBand							  ",
	L"NtUserDelegateCapturePointers						  ",
	L"NtUserDelegateInput",
	L"NtUserDestroyDCompositionHwndTarget					  ",
	L"NtUserDestroyInputContext							  ",
	L"NtUserDisableImmersiveOwner							  ",
	L"NtUserDisableProcessWindowFiltering					  ",
	L"NtUserDisableThreadIme								  ",
	L"NtUserDiscardPointerFrameMessages					  ",
	L"NtUserDisplayConfigGetDeviceInfo					  ",
	L"NtUserDisplayConfigSetDeviceInfo					  ",
	L"NtUserDoSoundConnect								  ",
	L"NtUserDoSoundDisconnect								  ",
	L"NtUserDragDetect",
	L"NtUserDragObject",
	L"NtUserDrawAnimatedRects								  ",
	L"NtUserDrawCaption",
	L"NtUserDrawCaptionTemp								  ",
	L"NtUserDrawMenuBarTemp								  ",
	L"NtUserDwmGetRemoteSessionOcclusionEvent				  ",
	L"NtUserDwmGetRemoteSessionOcclusionState				  ",
	L"NtUserDwmKernelShutdown								  ",
	L"NtUserDwmKernelStartup								  ",
	L"NtUserDwmValidateWindow								  ",
	L"NtUserEnableChildWindowDpiMessage					  ",
	L"NtUserEnableIAMAccess								  ",
	L"NtUserEnableMouseInPointer							  ",
	L"NtUserEnableMouseInputForCursorSuppression			  ",
	L"NtUserEnableNonClientDpiScaling						  ",
	L"NtUserEnableResizeLayoutSynchronization				  ",
	L"NtUserEnableTouchPad								  ",
	L"NtUserEnableWindowGDIScaledDpiMessage				  ",
	L"NtUserEnableWindowResizeOptimization				  ",
	L"NtUserEndMenu	",
	L"NtUserEvent		",
	L"NtUserFlashWindowEx",
	L"NtUserFrostCrashedWindow							  ",
	L"NtUserFunctionalizeDisplayConfig					  ",
	L"NtUserGetActiveProcessesDpis						  ",
	L"NtUserGetAppImeLevel								  ",
	L"NtUserGetAutoRotationState							  ",
	L"NtUserGetCIMSSM	",
	L"NtUserGetCaretPos",
	L"NtUserGetClipCursor",
	L"NtUserGetClipboardAccessToken						  ",
	L"NtUserGetClipboardViewer							  ",
	L"NtUserGetComboBoxInfo								  ",
	L"NtUserGetCurrentInputMessageSource					  ",
	L"NtUserGetCursor	",
	L"NtUserGetCursorDims",
	L"NtUserGetCursorInfo",
	L"NtUserGetDManipHookInitFunction						  ",
	L"NtUserGetDesktopID",
	L"NtUserGetDisplayAutoRotationPreferences				  ",
	L"NtUserGetDisplayAutoRotationPreferencesByProcessId	  ",
	L"NtUserGetDisplayConfigBufferSizes					  ",
	L"NtUserGetDpiForCurrentProcess						  ",
	L"NtUserGetDpiForMonitor								  ",
	L"NtUserGetGestureConfig								  ",
	L"NtUserGetGestureExtArgs								  ",
	L"NtUserGetGestureInfo								  ",
	L"NtUserGetGuiResources								  ",
	L"NtUserGetHDevName",
	L"NtUserGetHimetricScaleFactorFromPixelLocation		  ",
	L"NtUserGetImeHotKey",
	L"NtUserGetImeInfoEx",
	L"NtUserGetInputLocaleInfo							  ",
	L"NtUserGetInteractiveControlDeviceInfo				  ",
	L"NtUserGetInteractiveControlInfo						  ",
	L"NtUserGetInteractiveCtrlSupportedWaveforms			  ",
	L"NtUserGetInternalWindowPos							  ",
	L"NtUserGetKeyNameText								  ",
	L"NtUserGetKeyboardLayoutName							  ",
	L"NtUserGetLayeredWindowAttributes					  ",
	L"NtUserGetListBoxInfo								  ",
	L"NtUserGetMenuIndex",
	L"NtUserGetMenuItemRect								  ",
	L"NtUserGetMouseMovePointsEx							  ",
	L"NtUserGetPhysicalDeviceRect							  ",
	L"NtUserGetPointerCursorId							  ",
	L"NtUserGetPointerDevice								  ",
	L"NtUserGetPointerDeviceCursors						  ",
	L"NtUserGetPointerDeviceProperties					  ",
	L"NtUserGetPointerDeviceRects							  ",
	L"NtUserGetPointerDevices								  ",
	L"NtUserGetPointerFrameArrivalTimes					  ",
	L"NtUserGetPointerInfoList							  ",
	L"NtUserGetPointerInputTransform						  ",
	L"NtUserGetPointerType								  ",
	L"NtUserGetPrecisionTouchPadConfiguration				  ",
	L"NtUserGetPriorityClipboardFormat					  ",
	L"NtUserGetProcessDpiAwarenessContext					  ",
	L"NtUserGetProcessUIContextInformation				  ",
	L"NtUserGetQueueStatusReadonly						  ",
	L"NtUserGetRawInputBuffer								  ",
	L"NtUserGetRawInputData								  ",
	L"NtUserGetRawInputDeviceInfo							  ",
	L"NtUserGetRawInputDeviceList							  ",
	L"NtUserGetRawPointerDeviceData						  ",
	L"NtUserGetRegisteredRawInputDevices					  ",
	L"NtUserGetResizeDCompositionSynchronizationObject	  ",
	L"NtUserGetTopLevelWindow								  ",
	L"NtUserGetTouchInputInfo								  ",
	L"NtUserGetTouchValidationStatus						  ",
	L"NtUserGetUpdatedClipboardFormats					  ",
	L"NtUserGetWOWClass",
	L"NtUserGetWindowBand",
	L"NtUserGetWindowCompositionAttribute					  ",
	L"NtUserGetWindowCompositionInfo						  ",
	L"NtUserGetWindowDisplayAffinity						  ",
	L"NtUserGetWindowFeedbackSetting						  ",
	L"NtUserGetWindowMinimizeRect							  ",
	L"NtUserGetWindowRgnEx								  ",
	L"NtUserGhostWindowFromHungWindow						  ",
	L"NtUserHandleDelegatedInput							  ",
	L"NtUserHardErrorControl								  ",
	L"NtUserHidePointerContactVisualization				  ",
	L"NtUserHiliteMenuItem								  ",
	L"NtUserHungWindowFromGhostWindow						  ",
	L"NtUserHwndQueryRedirectionInfo						  ",
	L"NtUserHwndSetRedirectionInfo						  ",
	L"NtUserImpersonateDdeClientWindow					  ",
	L"NtUserInheritWindowMonitor							  ",
	L"NtUserInitTask	",
	L"NtUserInitialize",
	L"NtUserInitializeClientPfnArrays						  ",
	L"NtUserInitializeGenericHidInjection					  ",
	L"NtUserInitializeInputDeviceInjection				  ",
	L"NtUserInitializePointerDeviceInjection				  ",
	L"NtUserInitializePointerDeviceInjectionEx			  ",
	L"NtUserInitializeTouchInjection						  ",
	L"NtUserInjectDeviceInput								  ",
	L"NtUserInjectGenericHidInput							  ",
	L"NtUserInjectGesture",
	L"NtUserInjectKeyboardInput							  ",
	L"NtUserInjectMouseInput								  ",
	L"NtUserInjectPointerInput							  ",
	L"NtUserInjectTouchInput								  ",
	L"NtUserInteractiveControlQueryUsage					  ",
	L"NtUserInternalGetWindowIcon							  ",
	L"NtUserIsChildWindowDpiMessageEnabled				  ",
	L"NtUserIsMouseInPointerEnabled						  ",
	L"NtUserIsMouseInputEnabled							  ",
	L"NtUserIsNonClientDpiScalingEnabled					  ",
	L"NtUserIsResizeLayoutSynchronizationEnabled			  ",
	L"NtUserIsTopLevelWindow								  ",
	L"NtUserIsTouchWindow",
	L"NtUserIsWindowBroadcastingDpiToChildren				  ",
	L"NtUserIsWindowGDIScaledDpiMessageEnabled			  ",
	L"NtUserLayoutCompleted								  ",
	L"NtUserLinkDpiCursor",
	L"NtUserLoadKeyboardLayoutEx							  ",
	L"NtUserLockCursor",
	L"NtUserLockWindowStation								  ",
	L"NtUserLockWorkStation								  ",
	L"NtUserLogicalToPerMonitorDPIPhysicalPoint			  ",
	L"NtUserLogicalToPhysicalPoint						  ",
	L"NtUserMNDragLeave",
	L"NtUserMNDragOver",
	L"NtUserMagControl",
	L"NtUserMagGetContextInformation						  ",
	L"NtUserMagSetContextInformation						  ",
	L"NtUserMenuItemFromPoint								  ",
	L"NtUserMinMaximize",
	L"NtUserModifyWindowTouchCapability					  ",
	L"NtUserMsgWaitForMultipleObjectsEx					  ",
	L"NtUserNavigateFocus",
	L"NtUserNotifyIMEStatus								  ",
	L"NtUserOpenInputDesktop								  ",
	L"NtUserOpenThreadDesktop								  ",
	L"NtUserPaintMonitor",
	L"NtUserPerMonitorDPIPhysicalToLogicalPoint			  ",
	L"NtUserPhysicalToLogicalPoint						  ",
	L"NtUserPrintWindow",
	L"NtUserProcessInkFeedbackCommand						  ",
	L"NtUserPromoteMouseInPointer							  ",
	L"NtUserPromotePointer								  ",
	L"NtUserQueryBSDRWindow								  ",
	L"NtUserQueryDisplayConfig							  ",
	L"NtUserQueryInformationThread						  ",
	L"NtUserQueryInputContext								  ",
	L"NtUserQuerySendMessage								  ",
	L"NtUserRealChildWindowFromPoint						  ",
	L"NtUserRealWaitMessageEx								  ",
	L"NtUserRegisterBSDRWindow							  ",
	L"NtUserRegisterDManipHook							  ",
	L"NtUserRegisterEdgy",
	L"NtUserRegisterErrorReportingDialog					  ",
	L"NtUserRegisterHotKey								  ",
	L"NtUserRegisterManipulationThread					  ",
	L"NtUserRegisterPointerDeviceNotifications			  ",
	L"NtUserRegisterPointerInputTarget					  ",
	L"NtUserRegisterRawInputDevices						  ",
	L"NtUserRegisterServicesProcess						  ",
	L"NtUserRegisterSessionPort							  ",
	L"NtUserRegisterShellPTPListener						  ",
	L"NtUserRegisterTasklist								  ",
	L"NtUserRegisterTouchHitTestingWindow					  ",
	L"NtUserRegisterTouchPadCapable						  ",
	L"NtUserRegisterUserApiHook							  ",
	L"NtUserReleaseDC	",
	L"NtUserReleaseDwmHitTestWaiters						  ",
	L"NtUserRemoteConnect",
	L"NtUserRemoteRedrawRectangle							  ",
	L"NtUserRemoteRedrawScreen							  ",
	L"NtUserRemoteStopScreenUpdates						  ",
	L"NtUserRemoveClipboardFormatListener					  ",
	L"NtUserRemoveInjectionDevice							  ",
	L"NtUserReportInertia",
	L"NtUserResolveDesktopForWOW							  ",
	L"NtUserSendEventMessage								  ",
	L"NtUserSendInteractiveControlHapticsReport			  ",
	L"NtUserSetActivationFilter							  ",
	L"NtUserSetActiveProcessForMonitor					  ",
	L"NtUserSetAppImeLevel								  ",
	L"NtUserSetAutoRotation								  ",
	L"NtUserSetBrokeredForeground							  ",
	L"NtUserSetCalibrationData							  ",
	L"NtUserSetChildWindowNoActivate						  ",
	L"NtUserSetClassWord",
	L"NtUserSetCoreWindow",
	L"NtUserSetCoreWindowPartner							  ",
	L"NtUserSetCursorContents								  ",
	L"NtUserSetCursorPos",
	L"NtUserSetDesktopColorTransform						  ",
	L"NtUserSetDialogControlDpiChangeBehavior				  ",
	L"NtUserSetDisplayAutoRotationPreferences				  ",
	L"NtUserSetDisplayConfig								  ",
	L"NtUserSetDisplayMapping								  ",
	L"NtUserSetFallbackForeground							  ",
	L"NtUserSetFeatureReportResponse						  ",
	L"NtUserSetGestureConfig								  ",
	L"NtUserSetImeHotKey",
	L"NtUserSetImeInfoEx",
	L"NtUserSetImeOwnerWindow								  ",
	L"NtUserSetInteractiveControlFocus					  ",
	L"NtUserSetInteractiveCtrlRotationAngle				  ",
	L"NtUserSetInternalWindowPos							  ",
	L"NtUserSetLayeredWindowAttributes					  ",
	L"NtUserSetManipulationInputTarget					  ",
	L"NtUserSetMenu	",
	L"NtUserSetMenuContextHelpId							  ",
	L"NtUserSetMenuFlagRtoL								  ",
	L"NtUserSetMirrorRendering							  ",
	L"NtUserSetObjectInformation							  ",
	L"NtUserSetPrecisionTouchPadConfiguration				  ",
	L"NtUserSetProcessDpiAwarenessContext					  ",
	L"NtUserSetProcessInteractionFlags					  ",
	L"NtUserSetProcessRestrictionExemption				  ",
	L"NtUserSetProcessUIAccessZorder						  ",
	L"NtUserSetShellWindowEx								  ",
	L"NtUserSetSysColors",
	L"NtUserSetSystemCursor								  ",
	L"NtUserSetSystemTimer								  ",
	L"NtUserSetTargetForResourceBrokering					  ",
	L"NtUserSetThreadInputBlocked							  ",
	L"NtUserSetThreadLayoutHandles						  ",
	L"NtUserSetWindowArrangement							  ",
	L"NtUserSetWindowBand",
	L"NtUserSetWindowCompositionAttribute					  ",
	L"NtUserSetWindowCompositionTransition				  ",
	L"NtUserSetWindowDisplayAffinity						  ",
	L"NtUserSetWindowFeedbackSetting						  ",
	L"NtUserSetWindowRgnEx								  ",
	L"NtUserSetWindowShowState							  ",
	L"NtUserSetWindowStationUser							  ",
	L"NtUserShowCursor",
	L"NtUserShowSystemCursor								  ",
	L"NtUserShutdownBlockReasonCreate						  ",
	L"NtUserShutdownBlockReasonQuery						  ",
	L"NtUserShutdownReasonDestroy							  ",
	L"NtUserSignalRedirectionStartComplete				  ",
	L"NtUserSlicerControl",
	L"NtUserSoundSentry",
	L"NtUserStopAndEndInertia								  ",
	L"NtUserSwitchDesktop",
	L"NtUserSystemParametersInfoForDpi					  ",
	L"NtUserTestForInteractiveUser						  ",
	L"NtUserTrackPopupMenuEx								  ",
	L"NtUserTransformPoint								  ",
	L"NtUserTransformRect",
	L"NtUserUndelegateInput								  ",
	L"NtUserUnloadKeyboardLayout							  ",
	L"NtUserUnlockWindowStation							  ",
	L"NtUserUnregisterHotKey								  ",
	L"NtUserUnregisterSessionPort							  ",
	L"NtUserUnregisterUserApiHook							  ",
	L"NtUserUpdateDefaultDesktopThumbnail					  ",
	L"NtUserUpdateInputContext							  ",
	L"NtUserUpdateInstance								  ",
	L"NtUserUpdateLayeredWindow							  ",
	L"NtUserUpdatePerUserSystemParameters					  ",
	L"NtUserUpdateWindowInputSinkHints					  ",
	L"NtUserUpdateWindowTrackingInfo						  ",
	L"NtUserUserHandleGrantAccess							  ",
	L"NtUserValidateHandleSecure							  ",
	L"NtUserWOWCleanup",
	L"NtUserWaitAvailableMessageEx						  ",
	L"NtUserWaitForInputIdle								  ",
	L"NtUserWaitForMsgAndEvent							  ",
	L"NtUserWaitForRedirectionStartComplete				  ",
	L"NtUserWindowFromDC",
	L"NtUserWindowFromPhysicalPoint						  ",
	L"NtValidateCompositionSurfaceHandle					  ",
	L"NtVisualCaptureBits",
	L"NtUserSetClassLongPtr								  ",
	L"NtUserSetWindowLongPtr								  "
};
const WCHAR* const SSDT_FUNC_NAME[] =
{
	L"NtAccessCheck",
 L"NtWorkerFactoryWorkerReady",
 L"NtAcceptConnectPort",
 L"NtMapUserPhysicalPagesScatter",
 L"NtWaitForSingleObject",
 L"NtCallbackReturn",
 L"NtReadFile",
 L"NtDeviceIoControlFile",
 L"NtWriteFile",
 L"NtRemoveIoCompletion",
 L"NtReleaseSemaphore",
 L"NtReplyWaitReceivePort",
 L"NtReplyPort",
 L"NtSetInformationThread",
 L"NtSetEvent",
 L"NtClose",
 L"NtQueryObject",
 L"NtQueryInformationFile",
 L"NtOpenKey",
 L"NtEnumerateValueKey",
 L"NtFindAtom",
 L"NtQueryDefaultLocale",
 L"NtQueryKey",
 L"NtQueryValueKey",
 L"NtAllocateVirtualMemory",
 L"NtQueryInformationProcess",
 L"NtWaitForMultipleObjects32 ",
 L"NtWriteFileGather		  ",
 L"NtSetInformationProcess	  ",
 L"NtCreateKey				  ",
 L"NtFreeVirtualMemory		  ",
 L"NtImpersonateClientOfPort  ",
 L"NtReleaseMutant			  ",
 L"NtQueryInformationToken	  ",
 L"NtRequestWaitReplyPort	  ",
 L"NtQueryVirtualMemory		  ",
 L"NtOpenThreadToken		  ",
 L"NtQueryInformationThread	  ",
 L"NtOpenProcess			  ",
 L"NtSetInformationFile		  ",
 L"NtMapViewOfSection		  ",
 L"NtAccessCheckAndAuditAlarm ",
 L"NtUnmapViewOfSection		  ",
 L"NtReplyWaitReceivePortEx	  ",
 L"NtTerminateProcess		  ",
 L"NtSetEventBoostPriority	  ",
 L"NtReadFileScatter		  ",
 L"NtOpenThreadTokenEx		  ",
 L"NtOpenProcessTokenEx		  ",
 L"NtQueryPerformanceCounter  ",
 L"NtEnumerateKey			  ",
 L"NtOpenFile				  ",
 L"NtDelayExecution			  ",
 L"NtQueryDirectoryFile		  ",
 L"NtQuerySystemInformation	  ",
 L"NtOpenSection			  ",
 L"NtQueryTimer				  ",
 L"NtFsControlFile			  ",
 L"NtWriteVirtualMemory		  ",
 L"NtCloseObjectAuditAlarm	  ",
 L"NtDuplicateObject		  ",
 L"NtQueryAttributesFile	  ",
 L"NtClearEvent				  ",
 L"NtReadVirtualMemory		  ",
 L"NtOpenEvent				  ",
 L"NtAdjustPrivilegesToken	  ",
 L"NtDuplicateToken			  ",
 L"NtContinue				  ",
 L"NtQueryDefaultUILanguage	  ",
 L"NtQueueApcThread			  ",
 L"NtYieldExecution			  ",
 L"NtAddAtom				  ",
 L"NtCreateEvent			  ",
 L"NtQueryVolumeInformationFile	 ",
 L"NtCreateSection				 ",
 L"NtFlushBuffersFile			 ",
 L"NtApphelpCacheControl		 ",
 L"NtCreateProcessEx			 ",
 L"NtCreateThread				 ",
 L"NtIsProcessInJob				 ",
 L"NtProtectVirtualMemory		 ",
 L"NtQuerySection				 ",
 L"NtResumeThread				 ",
 L"NtTerminateThread			 ",
 L"NtReadRequestData			 ",
 L"NtCreateFile					 ",
 L"NtQueryEvent					 ",
 L"NtWriteRequestData			 ",
 L"NtOpenDirectoryObject		 ",				 
 L"NtAccessCheckByTypeAndAuditAlarm				 ",
 L"NtQuerySystemTime							 ",
 L"NtWaitForMultipleObjects						 ",
 L"NtSetInformationObject						 ",
 L"NtCancelIoFile								 ",
 L"NtTraceEvent									 ",
 L"NtPowerInformation							 ",
 L"NtSetValueKey								 ",
 L"NtCancelTimer								 ",
 L"NtSetTimer									 ",
 L"NtAccessCheckByType							 ",
 L"NtAccessCheckByTypeResultList				 ",
 L"NtAccessCheckByTypeResultListAndAuditAlarm	 ",
 L"NtAccessCheckByTypeResultListAndAuditAlarmByHandle  ",
 L"NtAcquireProcessActivityReference				   ",
 L"NtAddAtomEx	 ",
 L"NtAddBootEntry ",
 L"NtAddDriverEntry ",
 L"NtAdjustGroupsToken								   ",
 L"NtAdjustTokenClaimsAndDeviceGroups				   ",
 L"NtAlertResumeThread								   ",
 L"NtAlertThread ",
 L"NtAlertThreadByThreadId							   ",
 L"NtAllocateLocallyUniqueId						   ",
 L"NtAllocateReserveObject							   ",
 L"NtAllocateUserPhysicalPages						   ",
 L"NtAllocateUuids ",
 L"NtAlpcAcceptConnectPort							   ",
 L"NtAlpcCancelMessage								   ",
 L"NtAlpcConnectPort								   ",
 L"NtAlpcConnectPortEx								   ",
 L"NtAlpcCreatePort ",
 L"NtAlpcCreatePortSection							   ",
 L"NtAlpcCreateResourceReserve						   ",
 L"NtAlpcCreateSectionView							   ",
 L"NtAlpcCreateSecurityContext						   ",
 L"NtAlpcDeletePortSection							   ",
 L"NtAlpcDeleteResourceReserve						   ",
 L"NtAlpcDeleteSectionView							   ",
 L"NtAlpcDeleteSecurityContext						   ",
 L"NtAlpcDisconnectPort								   ",
 L"NtAlpcImpersonateClientContainerOfPort			   ",
 L"NtAlpcImpersonateClientOfPort					   ",
 L"NtAlpcOpenSenderProcess							   ",
 L"NtAlpcOpenSenderThread							   ",
 L"NtAlpcQueryInformation							   ",
 L"NtAlpcQueryInformationMessage					   ",
 L"NtAlpcRevokeSecurityContext						   ",
 L"NtAlpcSendWaitReceivePort",
 L"NtAlpcSetInformation",
 L"NtAreMappedFilesTheSame",
 L"NtAssignProcessToJobObject",
 L"NtAssociateWaitCompletionPacket",
 L"NtCallEnclave ",
 L"NtCancelIoFileEx ",
 L"NtCancelSynchronousIoFile						   ",
 L"NtCancelTimer2 ",
 L"NtCancelWaitCompletionPacket						   ",
 L"NtCommitComplete ",
 L"NtCommitEnlistment								   ",
 L"NtCommitRegistryTransaction						   ",
 L"NtCommitTransaction								   ",
 L"NtCompactKeys ",
 L"NtCompareObjects ",
 L"NtCompareSigningLevels							   ",
 L"NtCompareTokens ",
 L"NtCompleteConnectPort							   ",
 L"NtCompressKey ",
 L"NtConnectPort ",
 L"NtConvertBetweenAuxiliaryCounterAndPerformanceCounter ",
 L"NtCreateDebugObject									 ",
 L"NtCreateDirectoryObject								 ",
 L"NtCreateDirectoryObjectEx							 ",
 L"NtCreateEnclave										 ",
 L"NtCreateEnlistment									 ",
 L"NtCreateEventPair									 ",
 L"NtCreateIRTimer										 ",
 L"NtCreateIoCompletion									 ",
 L"NtCreateJobObject									 ",
 L"NtCreateJobSet										 ",
 L"NtCreateKeyTransacted								 ",
 L"NtCreateKeyedEvent									 ",
 L"NtCreateLowBoxToken									 ",
 L"NtCreateMailslotFile									 ",
 L"NtCreateMutant										 ",
 L"NtCreateNamedPipeFile								 ",
 L"NtCreatePagingFile									 ",
 L"NtCreatePartition									 ",
 L"NtCreatePort											 ",
 L"NtCreatePrivateNamespace								 ",
 L"NtCreateProcess										 ",
 L"NtCreateProfile										 ",
 L"NtCreateProfileEx									 ",
 L"NtCreateRegistryTransaction							 ",
 L"NtCreateResourceManager								 ",
 L"NtCreateSemaphore									 ",
 L"NtCreateSymbolicLinkObject							 ",
 L"NtCreateThreadEx										 ",
 L"NtCreateTimer										 ",
 L"NtCreateTimer2										 ",
 L"NtCreateToken										 ",
 L"NtCreateTokenEx										 ",
 L"NtCreateTransaction									 ",
 L"NtCreateTransactionManager							 ",
 L"NtCreateUserProcess									 ",
 L"NtCreateWaitCompletionPacket							 ",
 L"NtCreateWaitablePort									 ",
 L"NtCreateWnfStateName									 ",
 L"NtCreateWorkerFactory								 ",
 L"NtDebugActiveProcess									 ",
 L"NtDebugContinue										 ",
 L"NtDeleteAtom											 ",
 L"NtDeleteBootEntry									 ",
 L"NtDeleteDriverEntry									 ",
 L"NtDeleteFile											 ",
 L"NtDeleteKey											 ",
 L"NtDeleteObjectAuditAlarm								 ",
 L"NtDeletePrivateNamespace								 ",
 L"NtDeleteValueKey										 ",
 L"NtDeleteWnfStateData									 ",
 L"NtDeleteWnfStateName									 ",
 L"NtDisableLastKnownGood								 ",
 L"NtDisplayString										 ",
 L"NtDrawText											 ",
 L"NtEnableLastKnownGood								 ",
 L"NtEnumerateBootEntries								 ",
 L"NtEnumerateDriverEntries								 ",
 L"NtEnumerateSystemEnvironmentValuesEx					 ",
 L"NtEnumerateTransactionObject							 ",
 L"NtExtendSection										 ",
 L"NtFilterBootOption									 ",
 L"NtFilterToken										 ",
 L"NtFilterTokenEx										 ",
 L"NtFlushBuffersFileEx									 ",
 L"NtFlushInstallUILanguage								 ",
 L"NtFlushInstructionCache								 ",
 L"NtFlushKey											 ",
 L"NtFlushProcessWriteBuffers							 ",
 L"NtFlushVirtualMemory									 ",
 L"NtFlushWriteBuffer									 ",
 L"NtFreeUserPhysicalPages								 ",
 L"NtFreezeRegistry										 ",
 L"NtFreezeTransactions									 ",
 L"NtGetCachedSigningLevel								 ",
 L"NtGetCompleteWnfStateSubscription					 ",
 L"NtGetContextThread									 ",
 L"NtGetCurrentProcessorNumber							 ",
 L"NtGetCurrentProcessorNumberEx						 ",
 L"NtGetDevicePowerState								 ",
 L"NtGetMUIRegistryInfo									 ",
 L"NtGetNextProcess										 ",
 L"NtGetNextThread										 ",
 L"NtGetNlsSectionPtr									 ",
 L"NtGetNotificationResourceManager						 ",
 L"NtGetWriteWatch										 ",
 L"NtImpersonateAnonymousToken							 ",
 L"NtImpersonateThread									 ",
 L"NtInitializeEnclave									 ",
 L"NtInitializeNlsFiles									 ",
 L"NtInitializeRegistry									 ",
 L"NtInitiatePowerAction								 ",
 L"NtIsSystemResumeAutomatic							 ",
 L"NtIsUILanguageComitted								 ",
 L"NtListenPort											 ",
 L"NtLoadDriver											 ",
 L"NtLoadEnclaveData									 ",
 L"NtLoadHotPatch										 ",
 L"NtLoadKey											 ",
 L"NtLoadKey2											 ",
 L"NtLoadKeyEx											 ",
 L"NtLockFile											 ",
 L"NtLockProductActivationKeys							 ",
 L"NtLockRegistryKey									 ",
 L"NtLockVirtualMemory									 ",
 L"NtMakePermanentObject								 ",
 L"NtMakeTemporaryObject								 ",
 L"NtManagePartition									 ",
 L"NtMapCMFModule										 ",
 L"NtMapUserPhysicalPages								 ",
 L"NtModifyBootEntry									 ",
 L"NtModifyDriverEntry									 ",
 L"NtNotifyChangeDirectoryFile							 ",
 L"NtNotifyChangeDirectoryFileEx						 ",
 L"NtNotifyChangeKey									 ",
 L"NtNotifyChangeMultipleKeys							 ",
 L"NtNotifyChangeSession								 ",
 L"NtOpenEnlistment										 ",
 L"NtOpenEventPair										 ",
 L"NtOpenIoCompletion									 ",
 L"NtOpenJobObject										 ",
 L"NtOpenKeyEx											 ",
 L"NtOpenKeyTransacted									 ",
 L"NtOpenKeyTransactedEx								 ",
 L"NtOpenKeyedEvent										 ",
 L"NtOpenMutant											 ",
 L"NtOpenObjectAuditAlarm								 ",
 L"NtOpenPartition										 ",
 L"NtOpenPrivateNamespace								 ",
 L"NtOpenProcessToken									 ",
 L"NtOpenRegistryTransaction							 ",
 L"NtOpenResourceManager								 ",
 L"NtOpenSemaphore										 ",
 L"NtOpenSession										 ",
 L"NtOpenSymbolicLinkObject								 ",
 L"NtOpenThread											 ",
 L"NtOpenTimer											 ",
 L"NtOpenTransaction									 ",
 L"NtOpenTransactionManager								 ",
 L"NtPlugPlayControl									 ",
 L"NtPrePrepareComplete									 ",
 L"NtPrePrepareEnlistment								 ",
 L"NtPrepareComplete									 ",
 L"NtPrepareEnlistment									 ",
 L"NtPrivilegeCheck										 ",
 L"NtPrivilegeObjectAuditAlarm							 ",
 L"NtPrivilegedServiceAuditAlarm						 ",
 L"NtPropagationComplete								 ",
 L"NtPropagationFailed									 ",
 L"NtPulseEvent											 ",
 L"NtQueryAuxiliaryCounterFrequency						 ",
 L"NtQueryBootEntryOrder								 ",
 L"NtQueryBootOptions									 ",
 L"NtQueryDebugFilterState								 ",
 L"NtQueryDirectoryFileEx								 ",
 L"NtQueryDirectoryObject								 ",
 L"NtQueryDriverEntryOrder								 ",
 L"NtQueryEaFile										 ",
 L"NtQueryFullAttributesFile							 ",
 L"NtQueryInformationAtom								 ",
 L"NtQueryInformationByName								 ",
 L"NtQueryInformationEnlistment							 ",
 L"NtQueryInformationJobObject							 ",
 L"NtQueryInformationPort								 ",
 L"NtQueryInformationResourceManager					 ",
 L"NtQueryInformationTransaction						 ",
 L"NtQueryInformationTransactionManager					 ",
 L"NtQueryInformationWorkerFactory						 ",
 L"NtQueryInstallUILanguage								 ",
 L"NtQueryIntervalProfile								 ",
 L"NtQueryIoCompletion									 ",
 L"NtQueryLicenseValue									 ",
 L"NtQueryMultipleValueKey								 ",
 L"NtQueryMutant										 ",
 L"NtQueryOpenSubKeys									 ",
 L"NtQueryOpenSubKeysEx									 ",
 L"NtQueryPortInformationProcess						 ",
 L"NtQueryQuotaInformationFile							 ",
 L"NtQuerySecurityAttributesToken						 ",
 L"NtQuerySecurityObject								 ",
 L"NtQuerySecurityPolicy								 ",
 L"NtQuerySemaphore										 ",
 L"NtQuerySymbolicLinkObject							 ",
 L"NtQuerySystemEnvironmentValue						 ",
 L"NtQuerySystemEnvironmentValueEx						 ",
 L"NtQuerySystemInformationEx							 ",
 L"NtQueryTimerResolution								 ",
 L"NtQueryWnfStateData									 ",
 L"NtQueryWnfStateNameInformation						 ",
 L"NtQueueApcThreadEx									 ",
 L"NtRaiseException										 ",
 L"NtRaiseHardError										 ",
 L"NtReadOnlyEnlistment									 ",
 L"NtRecoverEnlistment									 ",
 L"NtRecoverResourceManager								 ",
 L"NtRecoverTransactionManager							 ",
 L"NtRegisterProtocolAddressInformation					 ",
 L"NtRegisterThreadTerminatePort						 ",
 L"NtReleaseKeyedEvent									 ",
 L"NtReleaseWorkerFactoryWorker							 ",
 L"NtRemoveIoCompletionEx								 ",
 L"NtRemoveProcessDebug									 ",
 L"NtRenameKey											 ",
 L"NtRenameTransactionManager							 ",
 L"NtReplaceKey											 ",
 L"NtReplacePartitionUnit								 ",
 L"NtReplyWaitReplyPort									 ",
 L"NtRequestPort										 ",
 L"NtResetEvent											 ",
 L"NtResetWriteWatch									 ",
 L"NtRestoreKey											 ",
 L"NtResumeProcess										 ",
 L"NtRevertContainerImpersonation						 ",
 L"NtRollbackComplete									 ",
 L"NtRollbackEnlistment									 ",
 L"NtRollbackRegistryTransaction						 ",
 L"NtRollbackTransaction								 ",
 L"NtRollforwardTransactionManager						 ",
 L"NtSaveKey											 ",
 L"NtSaveKeyEx											 ",
 L"NtSaveMergedKeys										 ",
 L"NtSecureConnectPort									 ",
 L"NtSerializeBoot										 ",
 L"NtSetBootEntryOrder									 ",
 L"NtSetBootOptions										 ",
 L"NtSetCachedSigningLevel								 ",
 L"NtSetCachedSigningLevel2								 ",
 L"NtSetContextThread									 ",
 L"NtSetDebugFilterState								 ",
 L"NtSetDefaultHardErrorPort							 ",
 L"NtSetDefaultLocale									 ",
 L"NtSetDefaultUILanguage								 ",
 L"NtSetDriverEntryOrder								 ",
 L"NtSetEaFile											 ",
 L"NtSetHighEventPair									 ",
 L"NtSetHighWaitLowEventPair							 ",
 L"NtSetIRTimer											 ",
 L"NtSetInformationDebugObject							 ",
 L"NtSetInformationEnlistment							 ",
 L"NtSetInformationJobObject							 ",
 L"NtSetInformationKey									 ",
 L"NtSetInformationResourceManager						 ",
 L"NtSetInformationSymbolicLink							 ",
 L"NtSetInformationToken								 ",
 L"NtSetInformationTransaction							 ",
 L"NtSetInformationTransactionManager					 ",
 L"NtSetInformationVirtualMemory						 ",
 L"NtSetInformationWorkerFactory						 ",
 L"NtSetIntervalProfile									 ",
 L"NtSetIoCompletion									 ",
 L"NtSetIoCompletionEx									 ",
 L"NtSetLdtEntries										 ",
 L"NtSetLowEventPair									 ",
 L"NtSetLowWaitHighEventPair							 ",
 L"NtSetQuotaInformationFile							 ",
 L"NtSetSecurityObject									 ",
 L"NtSetSystemEnvironmentValue							 ",
 L"NtSetSystemEnvironmentValueEx						 ",
 L"NtSetSystemInformation								 ",
 L"NtSetSystemPowerState								 ",
 L"NtSetSystemTime										 ",
 L"NtSetThreadExecutionState							 ",
 L"NtSetTimer2											 ",
 L"NtSetTimerEx											 ",
 L"NtSetTimerResolution									 ",
 L"NtSetUuidSeed										 ",
 L"NtSetVolumeInformationFile							 ",
 L"NtSetWnfProcessNotificationEvent						 ",
 L"NtShutdownSystem										 ",
 L"NtShutdownWorkerFactory								 ",
 L"NtSignalAndWaitForSingleObject						 ",
 L"NtSinglePhaseReject									 ",
 L"NtStartProfile										 ",
 L"NtStopProfile										 ",
 L"NtSubscribeWnfStateChange							 ",
 L"NtSuspendProcess										 ",
 L"NtSuspendThread										 ",
 L"NtSystemDebugControl									 ",
 L"NtTerminateEnclave									 ",
 L"NtTerminateJobObject									 ",
 L"NtTestAlert											 ",
 L"NtThawRegistry										 ",
 L"NtThawTransactions									 ",
 L"NtTraceControl										 ",
 L"NtTranslateFilePath									 ",
 L"NtUmsThreadYield										 ",
 L"NtUnloadDriver										 ",
 L"NtUnloadKey											 ",
 L"NtUnloadKey2											 ",
 L"NtUnloadKeyEx										 ",
 L"NtUnlockFile											 ",
 L"NtUnlockVirtualMemory								 ",
 L"NtUnmapViewOfSectionEx								 ",
 L"NtUnsubscribeWnfStateChange							 ",
 L"NtUpdateWnfStateData									 ",
 L"NtVdmControl											 ",
 L"NtWaitForAlertByThreadId								 ",
 L"NtWaitForDebugEvent									 ",
 L"NtWaitForKeyedEvent									 ",
 L"NtWaitForWorkViaWorkerFactory						 ",
 L"NtWaitHighEventPair									 ",
 L"NtWaitLowEventPair"
};

const WCHAR* const IPR_FUNC_NAME[] =
{
	L"IRP MJ CREATE",
	L"IRP MJ CREATE NAMED PIPE",
	L"IRP MJ CLOSE",
	L"IRP MJ READ",
	L"IRP MJ WRITE",
	L"IRP MJ QUERY INFORMATION",
	L"IRP MJ SET INFORMATION",
	L"IRP MJ QUERY EA",
	L"IRP MJ SET EA",
	L"IRP MJ FLUSH BUFFERS",
	L"IRP MJ QUERY VOLUME INFORMATION",
	L"IRP MJ SET VOLUME INFORMATION",
	L"IRP MJ DIRECTORY CONTROL",
	L"IRP MJ FILE SYSTEM CONTROL",
	L"IRP MJ DEVICE CONTROL",
	L"IRP MJ INTERNAL DEVICE CONTROL",
	L"IRP MJ SHUTDOWN",
	L"IRP MJ LOCK CONTROL",
	L"IRP MJ CLEANUP",
	L"IRP MJ CREATE MAILSLOT",
	L"IRP MJ QUERY SECURITY",
	L"IRP MJ SET SECURITY",
	L"IRP MJ POWER",
	L"IRP MJ SYSTEM CONTROL",
	L"IRP MJ DEVICE CHANGE",
	L"IRP MJ QUERY QUOTA",
	L"IRP MJ SET QUOTA",
	L"IRP MJ PNP"
};

const WCHAR* const IST_NAME[] =
{
	L"Divide Error",
	L"Debug",
	L"NMI Interrupt Not applicable",
	L"Breakpoint",
	L"Overflow",
	L"BOUND Range Exceeded",
	L"Invalid Opcode",
	L"Device Not Available",
	L"Double Fault",
	L"Coprocessor Segment Overrun", // Intel reserved
	L"Invalid TSS",
	L"Segment Not Present",
	L"Stack Fault",
	L"General Protection",
	L"Page-Fault",
	L"Intel reserved",
	L"x87 FPU Floating-Point",
	L"Alignment Check",
	L"Machine-Check",
	L"SIMD Floating-Point",
	L"Virtualization",
	L"Intel reserved",
	L"User Defined"
};

BOOL
SV_GetDriverInfo(
	_In_ HANDLE hDevice,
	_In_ DWORD dwIoControlCode,
	_In_reads_bytes_opt_(nInBufferSize) LPVOID lpInBuffer,
	_In_ DWORD nInBufferSize,
	_Out_writes_bytes_to_opt_(nOutBufferSize, *lpBytesReturned) LPVOID lpOutBuffer,
	_In_ DWORD nOutBufferSize,
	_Out_opt_ LPDWORD lpBytesReturned
)
{
	if (!DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, nullptr))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			DWORD ttt = 0;;
			DWORD Num;
			DWORD tempreturns = 0;
			if (DeviceIoControl(hDevice, dwIoControlCode, &ttt, sizeof(DWORD), &Num, sizeof(DWORD), &tempreturns, nullptr))
			{
				if(Num)
				{
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					*lpBytesReturned = Num;
				}
			}
			else
				SetLastError(ERROR_INVALID_PARAMETER);
			return FALSE;
		}
		else
			return FALSE;
	}
	return TRUE;
}

TCHAR* GetDevicePath(
	IN  LPGUID InterfaceGuid
)
{
	HDEVINFO HardwareDeviceInfo;
	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
	ULONG Length, RequiredLength = 0;
	BOOL bResult;

	HardwareDeviceInfo = SetupDiGetClassDevs(
		InterfaceGuid,
		NULL,
		NULL,
		(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	if (HardwareDeviceInfo == INVALID_HANDLE_VALUE) {
		printf("SetupDiGetClassDevs failed!\n");
		exit(1);
	}

	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	bResult = SetupDiEnumDeviceInterfaces(HardwareDeviceInfo,
		0,
		InterfaceGuid,
		0,
		&DeviceInterfaceData);

	if (bResult == FALSE) {
		/*
		LPVOID lpMsgBuf;

		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0,
		NULL
		)) {

		printf("Error: %s", (LPSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		}
		*/
		printf("SetupDiEnumDeviceInterfaces failed.Error Code: %d\n", GetLastError());

		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		exit(1);
	}

	SetupDiGetDeviceInterfaceDetail(
		HardwareDeviceInfo,
		&DeviceInterfaceData,
		NULL,
		0,
		&RequiredLength,
		NULL
	);

	DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, RequiredLength);

	if (DeviceInterfaceDetailData == NULL) {
		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	Length = RequiredLength;

	bResult = SetupDiGetDeviceInterfaceDetail(
		HardwareDeviceInfo,
		&DeviceInterfaceData,
		DeviceInterfaceDetailData,
		Length,
		&RequiredLength,
		NULL);

	if (bResult == FALSE) {
		/*
		LPVOID lpMsgBuf;

		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0,
		NULL
		)) {

		MessageBox(NULL, (LPCTSTR) lpMsgBuf, "Error", MB_OK);
		LocalFree(lpMsgBuf);
		}
		*/
		printf("Error in SetupDiGetDeviceInterfaceDetail\n");

		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		LocalFree(DeviceInterfaceDetailData);
		exit(1);
	}

	return DeviceInterfaceDetailData->DevicePath;

}

void Print(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PDWORD64 temp = (PDWORD64)Ptr;
	while (Count--)
	{
		if(*temp)
		cout << hex << *temp << endl;
		temp++;
	}
}

void PrintBug(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PBUG_STRUCT temp = (PBUG_STRUCT)Ptr;
	while (Count--)
	{
		if (temp->BUG_Address)
		{
			cout << hex << temp->BUG_Address << "	" << temp->DriverName << endl;
		}
		temp++;
	}
}

void PrintShutdown(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PSHUTDOWN_STRUCT temp = (PSHUTDOWN_STRUCT)Ptr;
	while (Count--)
	{
		if (temp->SHUTDOWN)
		cout << hex << temp->SHUTDOWN << "	DEVICE OBJECT:"<< temp->DEVICE_OBJECT << endl;
		temp++;
	}
}
void PrintPlugplay(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PPLUGPLAY_STRUCT temp = (PPLUGPLAY_STRUCT)Ptr;
	while (Count--)
	{
		if (temp->PlugPlay_Address)
		{
			cout << hex << temp->PlugPlay_Address << "	";
			wcout << temp->DriverName << endl;
		}
		temp++;
	}
}

void PrintFilter(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PFILTERSYS_INFO temp = (PFILTERSYS_INFO)Ptr;
	while (Count--)
	{
		if (temp->SysName && temp->HostSysName)
		{
			wcout << L"SysName:" << temp->SysName << L"	HostSysName:" << temp->HostSysName << L"	DeviceObject:" << hex << (DWORD64)temp->AttachDevice << endl;
		}
		temp++;
	}
}

void PrintDPC(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PDPCTIMER_STRUCT temp = (PDPCTIMER_STRUCT)Ptr;
	while (Count--)
	{
		if (temp->DeferredRoutine)
		{
			wcout << L"DPC Routine:" << hex << temp->DeferredRoutine << L"	DPC:" << temp->dpc << L"	DriverName:" << temp->DriverName << L"	Peroid:" << temp->Period << L"	Time:" << dec << temp->DueTime << endl;
		}
		temp++;
	}
}

void PrintGDT(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PGPTABLE_STRUCT temp = (PGPTABLE_STRUCT)Ptr;
	cout << "Sel        Base             Limit             Type   DPl Size Gran Pres Long Flags" << endl;//CPU SN 
	cout << "---- ---------------- ---------------- ------------- --- ---- ---- ---- ---- --------" << endl << endl;//--- -- 
	while (Count--)
	{
		if (temp)
		{
			printf("%04x %016llX %016llX %13s %03x %s   %s   %s   %s   0x%04x\n",
				temp->Index, //sizeof (KGDTENTRY)
				temp->Base,
				temp->Limit,
				temp->SegmentTypes,
				temp->DPl,
				temp->size,
				temp->Granularity,
				temp->Present,
				temp->LongMode,
				temp->Flags
				);
		}
		temp++;
	}
}

void PrintIDT(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	std::wcout.imbue(std::locale("chs"));
	wcout << Name << L"	Nums:" << Count << endl;
	PIDTABLE_STRUCT temp = (PIDTABLE_STRUCT)Ptr;
	for(DWORD i = 0; i < Count; i++)
	{
		if (i <= 20)
			wcout << L"The CPU Num:" << dec << temp->CPUNum << L"	The Interrupt Num:" << i << L"	Function Name:" << IST_NAME[i] << L"	Address:" << hex << temp->InterruptAddress << endl;//还可以进一步获取Stack的信息。P2863
		else if (i > 20 && i <= 32)
			wcout << L"The CPU Num:" << dec << temp->CPUNum << L"	The Interrupt Num:" << i << L"	Function Name:" << IST_NAME[21] << L"	Address:" << hex << temp->InterruptAddress << endl;//还可以进一步获取Stack的信息。P2863
		else
			wcout << L"The CPU Num:" << dec << temp->CPUNum << L"	The Interrupt Num:" << i << L"	Function Name:" << IST_NAME[22] << L"	Address:" << hex << temp->InterruptAddress << endl;//还可以进一步获取Stack的信息。P2863
		temp++;
	}
}

void PrintHook(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PDWORD64 temp = (PDWORD64)Ptr;
	for (DWORD i = 0; i < Count; i++)
	{
		if (temp)
		{
			if (i > 27)
				wcout << IPR_FUNC_NAME[i - 28] << "					Address:" << hex << *temp << endl;
			else
				wcout << IPR_FUNC_NAME[i] << "					Address:" << hex << *temp << endl;
		}
		temp++;
	}
}

void PrintSSDT(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PDWORD64 temp = (PDWORD64)Ptr;
	for (DWORD i = 0; i < Count; i++)
	{
		if (temp)
		{
			wcout << SSDT_FUNC_NAME[i] << "	Address:" << *temp << endl;
		}
		temp++;
	}
}

void PrintSHADOWSSDT(PVOID Ptr, DWORD Count, WCHAR* Name = nullptr)
{
	wcout << Name << L"	Nums:" << Count << endl;
	PDWORD64 temp = (PDWORD64)Ptr;
	for (DWORD i = 0; i < Count; i++)
	{
		if (temp)
		{
			wcout << SHADOWSSDT_FUNC_NAME[i] << "	Address:" << *temp << endl;
		}
		temp++;
	}
}

int main()
{

	SYSTEM_INFO qwe = {};
	GetSystemInfo(&qwe);

	vector<DRIVER_MODULE> Driver_Module;
	std::wcout.imbue(std::locale("chs"));
	TCHAR* G_DevicePath = GetDevicePath((LPGUID)&GUID_DEVINTERFACE_SystemTest);
	HANDLE hdevice = CreateFile(
		G_DevicePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (hdevice == INVALID_HANDLE_VALUE) {
		GetLastError();
		return 0;
	}
	DWORD a = 4;
	DWORD ModuleNum = 0;

	DWORD buffersize = 0;
	DWORD buffersize2 = 0;
	PDWORD64 OutBuffer = nullptr;
	DWORD OutBuffersize = 0;
	DWORD64 InputBuffer = 0;
	DWORD InputBuffersize = 0;

// 	//MSG HOOK FAILED
// 	UINT64 pgSharedInfo = 0;
// 	HMODULE t = LoadLibrary(L"user32.dll");
// 	HMODULE ha = GetModuleHandle(L"user32.dll");
// 	pgSharedInfo = (UINT64)GetProcAddress(ha, "gSharedInfo");
// 	if (!DeviceIoControl(hdevice, MSGHOOK_GETADDR, &pgSharedInfo, 8, nullptr, 0, nullptr, nullptr))
// 	{
// 		GetLastError();
// 		if (t)	FreeLibrary(t);
// 		return CloseHandle(hdevice);
// 	}
//	if (t)	FreeLibrary(t);


	//HOOK
	while (!SV_GetDriverInfo(hdevice, GETSSDTSHADOWTABLE, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintSHADOWSSDT(OutBuffer, OutBuffersize / 8, L"Shadow SSDT");
	SafeFree(OutBuffer, OutBuffersize);

	while (!SV_GetDriverInfo(hdevice, GETSSDTABLE, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintSSDT(OutBuffer, OutBuffersize / 8, L"SSDT");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETTDXHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Tdx");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETNSIPROXYHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"NsiProxy");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETTCPIPHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Tcpip");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * 2 * sizeof(DWORD64));
	OutBuffersize = 0x1C * 2 * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETFSDHOOKLIST, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"FSD");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETMOUSEHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Mouse");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETKBDHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Keybroad");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETI8042PRTHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"I8042Prt");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETPARTMGRHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Partmgr");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETDISKHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"Disk");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETATAPIHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"ATAPI");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETACPIHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"ACPI");
	SafeFree(OutBuffer, OutBuffersize);

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1C * sizeof(DWORD64));
	OutBuffersize = 0x1C * sizeof(DWORD64);
	if (!DeviceIoControl(hdevice, GETSCSIHOOK, nullptr, 0, OutBuffer, OutBuffersize, &buffersize, nullptr))
	{
		GetLastError();
	}
	PrintHook(OutBuffer, OutBuffersize / sizeof(DWORD64), L"SCSI");
	SafeFree(OutBuffer, OutBuffersize);

	//IDT
 	for (UINT Index = 0; Index < qwe.dwNumberOfProcessors; Index++)
	{
		while (!SV_GetDriverInfo(hdevice, GETIDTABLE, &Index, sizeof(int), OutBuffer, OutBuffersize, &buffersize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (OutBuffer)
				{
					HeapFree(GetProcessHeap(), 0, OutBuffer);
					OutBuffer = nullptr;
				}
				OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
				OutBuffersize = buffersize;
			}
			else
			{
				return CloseHandle(hdevice);
			}
		}
		//PrintIDT(OutBuffer, OutBuffersize / sizeof(IDTABLE_STRUCT), L"GDT");
	}
	SafeFree(OutBuffer, OutBuffersize);
	

	//GPT
	for (UINT Index = 0; Index < qwe.dwNumberOfProcessors; Index++)
	{
		while (!SV_GetDriverInfo(hdevice, GETGPTABLE, &Index, sizeof(int), OutBuffer, OutBuffersize, &buffersize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (OutBuffer)
				{
					HeapFree(GetProcessHeap(), 0, OutBuffer);
					OutBuffer = nullptr;
				}
				OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
				OutBuffersize = buffersize;
			}
			else
			{
				return CloseHandle(hdevice);
			}
		}
		PrintGDT(OutBuffer, OutBuffersize / sizeof(GPTABLE_STRUCT), L"GDT");
	}

	SafeFree(OutBuffer, OutBuffersize);
	
	//DPC
	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256 * sizeof(DPCTIMER_STRUCT));
	OutBuffersize = 256 * sizeof(DPCTIMER_STRUCT);
	if (!DeviceIoControl(hdevice, EnumDPCTimer, nullptr, 0, OutBuffer, OutBuffersize, nullptr, nullptr))
	{
		GetLastError();
		return CloseHandle(hdevice);
	}
	PrintDPC(OutBuffer, OutBuffersize / sizeof(DPCTIMER_STRUCT), L"Dpc Timer");
	SafeFree(OutBuffer, OutBuffersize);
	//FilterDriver

	OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 30 * sizeof(FILTERSYS_INFO));
	OutBuffersize = 30 * sizeof(FILTERSYS_INFO);
	while(!SV_GetDriverInfo(hdevice, EnumFilterDriver, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintFilter(OutBuffer, OutBuffersize / sizeof(FILTERSYS_INFO), L"Filter");
	SafeFree(OutBuffer, OutBuffersize);
	//CallBack

	while(!SV_GetDriverInfo(hdevice, ProcessorCallBack, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	Print((PVOID)OutBuffer, buffersize / 8, L"Create Process");
	SafeFree(OutBuffer, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, ThreadCallback, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	Print((PVOID)OutBuffer, buffersize / 8, L"Create Thread");
	SafeFree(OutBuffer, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, LoadImageCallback, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	Print((PVOID)OutBuffer, buffersize / 8, L"Load Image");
	SafeFree(OutBuffer, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, CmpCallback, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	Print((PVOID)OutBuffer, buffersize / 8, L"Cmp CallBack");
	SafeFree(OutBuffer, OutBuffersize);

	PBUG_STRUCT bug = nullptr;
	OutBuffersize = 0;
	while(!SV_GetDriverInfo(hdevice, BugCallback, nullptr, 0, bug, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (bug)
			{
				HeapFree(GetProcessHeap(), 0, bug);
				bug = nullptr;
			}
			bug = (PBUG_STRUCT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintBug(bug, OutBuffersize / sizeof(BUG_STRUCT), L"BUG");
	SafeFree(bug, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, BugReasonCallback, nullptr, 0, bug, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (bug)
			{
				HeapFree(GetProcessHeap(), 0, bug);
				bug = nullptr;
			}
			bug = (PBUG_STRUCT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintBug(bug, OutBuffersize / sizeof(BUG_STRUCT), L"BUG Reason");
	SafeFree(bug, OutBuffersize);

	PSHUTDOWN_STRUCT shutdown = nullptr;
	OutBuffersize = 0;
	while(!SV_GetDriverInfo(hdevice, ShutdownCallback, nullptr, 0, shutdown, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (shutdown)
			{
				HeapFree(GetProcessHeap(), 0, shutdown);
				shutdown = nullptr;
			}
			shutdown = (PSHUTDOWN_STRUCT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintShutdown(shutdown, OutBuffersize / sizeof(SHUTDOWN_STRUCT), L"Shutdown");
	SafeFree(shutdown, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, LastShutdownCallback, nullptr, 0, shutdown, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (shutdown)
			{
				HeapFree(GetProcessHeap(), 0, shutdown);
				shutdown = nullptr;
			}
			shutdown = (PSHUTDOWN_STRUCT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintShutdown(shutdown, OutBuffersize / sizeof(SHUTDOWN_STRUCT), L"Last Shutdown");
	SafeFree(shutdown, OutBuffersize);

	PPLUGPLAY_STRUCT plugplay = nullptr;
	while(!SV_GetDriverInfo(hdevice, PlugPlayCallback, nullptr, 0, plugplay, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (plugplay)
			{
				HeapFree(GetProcessHeap(), 0, plugplay);
				OutBuffer = nullptr;
			}
			plugplay = (PPLUGPLAY_STRUCT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	SafeFree(plugplay, OutBuffersize);

	while (!SV_GetDriverInfo(hdevice, FsNotifyChangeCallback, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintPlugplay((PVOID)OutBuffer, buffersize / sizeof(PLUGPLAY_STRUCT) , L"FsNotify Change");
	SafeFree(OutBuffer, OutBuffersize);

	while(!SV_GetDriverInfo(hdevice, PowerSettingCallback, nullptr, 0, OutBuffer, OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				HeapFree(GetProcessHeap(), 0, OutBuffer);
				OutBuffer = nullptr;
			}
			OutBuffer = (PDWORD64)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			OutBuffersize = buffersize;
		}
		else
		{
			return CloseHandle(hdevice);
		}
	}
	PrintPlugplay((PVOID)OutBuffer, buffersize / sizeof(PLUGPLAY_STRUCT), L"Power Setting");
	SafeFree(OutBuffer, OutBuffersize);

	//Module
	if (!DeviceIoControl(hdevice, GetModuleNum, nullptr, 0, &ModuleNum, sizeof(DWORD), nullptr, nullptr))
	{
		GetLastError();
		return CloseHandle(hdevice);
	}

	PDRIVER_MODULE module = new DRIVER_MODULE{};
	PDRIVER_MODULE firstmodule = module;
	for (DWORD i = 0; i < ModuleNum; i++)
	{
		a = 4;
		PDRIVER_MODULE CurMod = module;
		PDRIVER_MODULE temp = new DRIVER_MODULE{};
		CurMod->Next = temp;
		module = CurMod->Next;
		while (!DeviceIoControl(hdevice,
			EnumModule,
			CurMod,
			sizeof(DRIVER_MODULE),
			module,
			sizeof(DRIVER_MODULE),
			nullptr,
			nullptr) && a--)
		{ 
			GetLastError();
		}
		Driver_Module.emplace_back(*module);
	}

	{
			PDRIVER_MODULE Nextmodule = firstmodule->Next;
		delete firstmodule;
		firstmodule = Nextmodule;
		a = 0;
		while (Nextmodule)
		{
			a++;
			if (Nextmodule->address.Type == DRIVEROBJ)
			{
				std::cout << std::hex << std::setfill('0') << std::setw(16) << "Driver Object Address	" << Nextmodule->address.Driver_Object_Address << std::endl;
			}
			else
			{
				std::cout << std::setw(16) << "Driver Object Address	" << "-------------" << std::endl;
			}
			std::cout << std::hex << std::setfill('0') << std::setw(16) << "Driver Entry Address	" << Nextmodule->Driver_Entry << std::endl;
			std::cout << std::hex << std::setfill('0') << std::setw(16) << "Driver Dll Base Address	" << Nextmodule->Driver_DllBase << std::endl;
			std::cout << std::hex << std::setfill('0') << std::setw(16) << "Driver Object Size " << Nextmodule->SizeOfImage << std::endl;
			std::wcout << L"Driver Full Name: " << Nextmodule->FullDllName << std::endl;
			std::wcout << L"Driver Service Name: " << Nextmodule->ServiceName << std::endl;
			Nextmodule = Nextmodule->Next;
		}
	}

	//Process
	DWORD ProcessNum = 0;
	if (!DeviceIoControl(hdevice, GetProcessNum, nullptr, 0 , &ProcessNum, sizeof(DWORD), nullptr, nullptr))
	{
		GetLastError();
		return CloseHandle(hdevice);
	}

	PM_EPROCESS process = new M_EPROCESS{};
	PM_EPROCESS begin = process;

	DWORD nOutdata = 0;
	for (DWORD i = 0; i < ProcessNum; i++)
	{
		a = 4;
		PM_EPROCESS currPro = process;
		PM_EPROCESS temp = new M_EPROCESS{};
		currPro->Next = temp;
		process = currPro->Next;
		while (!DeviceIoControl(hdevice,
			EnumProc,
			currPro,
			sizeof(M_EPROCESS),
			process,
			sizeof(M_EPROCESS),
			&nOutdata,
			nullptr) && a--)
		{
			if (GetLastError() == ERROR_MORE_DATA)
			{
				if (process)
					delete process;
				currPro->Next = process;
				process = new M_EPROCESS{};
			}
		}

	}


	PM_EPROCESS Next = begin->Next;
	delete begin;
	begin = Next;
	a = 0;
	while (Next)
	{
		a++;
		std::cout << std::hex << "EPROCESS	" << Next->EProcess_ADDR << std::endl;
		std::cout << std::dec << "PID: " << Next->UniqueProcessId << "	Parent ID: " << Next->ParentProcessID << std::endl;
		std::wstring w(Next->FullName);
		std::wcout << L"Full Name: " << Next->FullName << std::endl;
		Next = Next->Next;
	}

	if (hdevice != INVALID_HANDLE_VALUE) {
		CloseHandle(hdevice);
	}

	return 0;
}

