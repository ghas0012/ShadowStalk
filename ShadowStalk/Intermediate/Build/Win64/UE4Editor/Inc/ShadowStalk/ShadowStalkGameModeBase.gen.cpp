// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShadowStalk/ShadowStalkGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeShadowStalkGameModeBase() {}
// Cross Module References
	SHADOWSTALK_API UClass* Z_Construct_UClass_AShadowStalkGameModeBase_NoRegister();
	SHADOWSTALK_API UClass* Z_Construct_UClass_AShadowStalkGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_ShadowStalk();
// End Cross Module References
	void AShadowStalkGameModeBase::StaticRegisterNativesAShadowStalkGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_AShadowStalkGameModeBase_NoRegister()
	{
		return AShadowStalkGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AShadowStalkGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AShadowStalkGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_ShadowStalk,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AShadowStalkGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "ShadowStalkGameModeBase.h" },
		{ "ModuleRelativePath", "ShadowStalkGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AShadowStalkGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AShadowStalkGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AShadowStalkGameModeBase_Statics::ClassParams = {
		&AShadowStalkGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AShadowStalkGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AShadowStalkGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AShadowStalkGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AShadowStalkGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AShadowStalkGameModeBase, 2292096107);
	template<> SHADOWSTALK_API UClass* StaticClass<AShadowStalkGameModeBase>()
	{
		return AShadowStalkGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AShadowStalkGameModeBase(Z_Construct_UClass_AShadowStalkGameModeBase, &AShadowStalkGameModeBase::StaticClass, TEXT("/Script/ShadowStalk"), TEXT("AShadowStalkGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AShadowStalkGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
