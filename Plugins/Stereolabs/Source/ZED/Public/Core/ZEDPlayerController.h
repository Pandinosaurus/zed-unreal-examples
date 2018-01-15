//======= Copyright (c) Stereolabs Corporation, All rights reserved. ===============

#pragma once

#include "Stereolabs/Public/Utilities/StereolabsViewportHelper.h"

#include "ZED/Public/Core/ZEDCamera.h"
#include "ZED/Public/Core/ZEDPawn.h"
#include "ZED/Public/Core/ZEDBaseTypes.h"

#include <sl_mr_core/Rendering.hpp>

#include "ZEDPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ZEDPlayerController, Log, All);

/*
 * Simple delegate
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZEDPlayerControllerDelegate);

/*
 * Base class for controller using the Zed.
 */
UCLASS(Category = "Stereolabs|Zed")
class ZED_API AZEDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZEDPlayerController();

	virtual void PostRenderFor(class APlayerController* PC, class UCanvas* Canvas, FVector CameraPosition, FVector CameraDir) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Zed")
	void AddShowOnlyComponent(UPrimitiveComponent* InComponent);

	/**
	 * Builds a list of components that are shown based upon gameplay.
	 * @param ShowOnlyComponentsOut this list will have all components that should be shown added to it
	 */
	void BuildShowOnlyComponentList(TSet<FPrimitiveComponentId>& ShowOnlyComponentsOut);

public:
	/*
	 * Open the camera
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void OpenZedCamera(bool bHideWorld = true);

	/*
	 * Close the camera
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void CloseZedCamera();

public:
	/*
	 * Spawn the pawn
	 * @return The pawn
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	UObject* SpawnPawn(UClass* NewPawnClass, bool bPossess = true);

	/*
	 * Spawn the zed actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void SpawnZedCameraActor();

	/*
	 * Fade in rendering
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void FadeIn();

	/*
	 * Fade out rendering
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void FadeOut();

	/*
	 * Initialize controller
	 * @param bOpenCamera      Open the camera automatically after controller initialized
	 * @param bStereoRendering Enable the HMD for stereo rendering
	 */
	UFUNCTION(BlueprintCallable, Category = "Zed")
	void Init();

	/*
	 * Event called after internal init
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Init"), Category = "Zed")
	void InitEvent();

	/*
	 * Called before opening Zed
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDOpeningZed();
	virtual void UpdateHUDOpeningZed_Implementation();

	/*
	 * Called every 2 seconds to check camera opening status
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDCheckOpeningZed();
	virtual void UpdateHUDCheckOpeningZed_Implementation();

	/*
	 * Called after Zed opened if tracking is not enabled in tracking parameters
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDZedOpened();
	virtual void UpdateHUDZedOpened_Implementation();

	/*
	 * Called after Zed opened if tracking is enabled in tacking parameters
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDEnablingZedTracking();
	virtual void UpdateHUDEnablingZedTracking_Implementation();

	/*
	 * Called after tracking enabled
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDZedTrackingEnabled(bool bSuccess, ESlErrorCode ErrorCode);
	virtual void UpdateHUDZedTrackingEnabled_Implementation(bool bSuccess, ESlErrorCode ErrorCode);

	/*
	 * Called if camera disconnected
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Zed")
	void UpdateHUDZedDisconnected();
	virtual void UpdateHUDZedDisconnected_Implementation();

private:
	/*
	 * Camera opened
	 */
	UFUNCTION()
	void ZedCameraOpened();

	/*
	 * Camera closed
	 */
	UFUNCTION()
	void ZedCameraClosed();

	/*
	 * Camera disconnected
	 */
	UFUNCTION()
	void ZedCameraDisconnected();

	/*
	 * Tracking enabled
	 * @param bSuccess  True if enabled
	 * @param ErrorCode The error code if failed
	 */
	UFUNCTION()
	void ZedCameraTrackingEnabled(bool bSuccess, ESlErrorCode ErrorCode, const FVector& Location, const FRotator& Rotation);

	/*
	 * Timeline fade function
	 */
	UFUNCTION()
	void Fading(float FadingFactor);

	/*
	 * Called after camera fully initialized
	 */
	UFUNCTION()
	void ZedCameraActorInitialized();

	/*
	 * Reset HMD tracking to be close to world origin
	 */
	UFUNCTION()
	void ResetHMDTrackingOrigin();

	/*
	 * Network notification for pawn spawning
	 */
	UFUNCTION()
	void OnRep_ZedPawn();

	/*
	 * Open the camera
	 */
	UFUNCTION()
	void Internal_OpenZedCamera();


	/*
	 * Close the camera
	 */
	void Internal_CloseZedCamera();

private:
	/*
	 * Disable fade post process
	 */
	void DisableFadePostProcess();

	/*
	 * Reset rendering and show error message
	 */
	void Internal_ZedCameraDisconnected();

	/*
	 * Update noise in post process
	 */
	void UpdateNoise();

	/*
	 * Initialize controller
	 */
	void Internal_Init();

	/*
	 * Open Camera
	 */
	void Internal_InitializeZedCameraActor();

	/*
	 * Fade out to game
	 */
	void FadeOutToGame();

public:
	/** The pawn class to spawn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	TSubclassOf<AZEDPawn> PawnClass;

	/** Pawn spawned delegate */
	UPROPERTY(BlueprintAssignable, Category = "Zed")
	FZEDPlayerControllerDelegate OnPawnSpawned;

	/** Zed camera actor initialization */
	UPROPERTY(BlueprintAssignable, Category = "Zed")
	FZEDPlayerControllerDelegate OnPreZedCameraOpening;

	/** The current Zed pawn possessed */
	UPROPERTY(BlueprintReadOnly, Category = "Zed", ReplicatedUsing = OnRep_ZedPawn)
	AZEDPawn* ZedPawn;

	/** The current Zed actor attached to the pawn */
	UPROPERTY(BlueprintReadOnly, Category = "Zed")
	AZEDCamera* ZedCamera;

	/** True to use pawn and Zed default spawn sequence and start initialization of the controller after pawn spawned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	bool bUseDefaultBeginPlay;

	/** True to open the camera after initialization */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	bool bOpenZedCameraAtInit;

	/** True if stereo rendering is supported. If true the controller will automatically open th HMD before opening the camera if any HMD is connected. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	bool bStereoRenderingSupport;

	/** True if player 1 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsFirstPlayer;

	/** Whether to render primitives component. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	bool bUseShowOnlyList;

	/** True to hide the world when opening the camera */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zed")
	bool bHideWorldOpeningZedCamera;

	/** Viewport helper */
	UPROPERTY(BlueprintReadOnly)
	FSlViewportHelper ViewportHelper;

private:
	/** Array of primitive components that will be only visible by the main camera */
	TArray<TWeakObjectPtr<UPrimitiveComponent>> ShowOnlyPrimitiveComponents;

	/** Previous noise factors */
	FZEDNoiseFactors LastNoiseFactors;

	/** Timer handle for the noise */
	FTimerHandle NoiseTimerHandle;

	/** Fade function */
	FOnTimelineFloat FadeFunction;

	/** Check camera opening timer handle */
	FTimerHandle CameraOpeningTimerHandle;

	/** Init controller timer handle */
	FTimerHandle InitTimerHandle;

	/** Disable fade post process timer handle */
	FTimerHandle DisableFadePostProcessTimerHandle;

	/** Camera disconnected timer handle */
	FTimerHandle CameraDisconnectedTimerHandle;

	/** Initialize Zed camera actor timer handle */
	FTimerHandle InitializeZedCameraActorTimerHandle;

	/** Fade out to game timer handle */
	FTimerHandle FadeOutTimerHandle;

	/** Reset HMD tracking origin timer handle */
	FTimerHandle ResetHMDTrackingOriginTimerHandle;

	/** Open the Zed camera after HMD enabled timer handle */
	FTimerHandle OpenZedCameraTimerHandle;

	/** Close the Zed camera after fade */
	FTimerHandle CloseZedCameraTimerHandle;

	/** Current FPS timer */
	float ZEDFPS;

	/** Current FPS timer */
	float CurrentFPSTimerBadFPS;

	/** Current FPS timer */
	float CurrentFPSTimerGoodFPS;

	/** Current camera FPS timer */
	float CurrentCameraFPSTimerBadFPS;

	/** Current camera FPS timer */
	float CurrentCameraFPSTimerGoodFPS;

	/** Class of the Zed camera blueprint */
	UPROPERTY()
	UClass* ZedCameraBlueprintClass;

	/** Dynamic instance of the post process fade material */
	UPROPERTY()
	UMaterialInstanceDynamic* PostProcessFadeMaterialInstanceDynamic;

	/** Post process material resource */
	UPROPERTY()
	UMaterial* PostProcessFadeSourceMaterial;

	/** Dynamic instance of the post process ZED material */
	UPROPERTY()
	UMaterialInstanceDynamic* PostProcessZedMaterialInstanceDynamic;

	/** Post process material resource */
	UPROPERTY()
	UMaterial* PostProcessZedSourceMaterial;

	/** Fade timeline */
	UPROPERTY()
	UTimelineComponent* FadeTimeline;

	/** Fade timeline curve */
	UPROPERTY()
	UCurveFloat* FadeTimelineCurve;

	/** Noise enable */
	int32 CurrentNoiseValue;

	/** True to tick the camera */
	uint8 bTickZedCamera:1;

	/** HMD enabled */
	uint8 bHMDEnabled:1;

	/** True if initialized */
	uint8 bInit:1;

	/** True to show low camera fps message */
	uint8 bShowLowCameraFPS:1;

	/** True to show low app fps message */
	uint8 bShowLowAppFPS:1;

	/** True if camera disconnected */
	uint8 bZedCameraDisconnected:1;
};