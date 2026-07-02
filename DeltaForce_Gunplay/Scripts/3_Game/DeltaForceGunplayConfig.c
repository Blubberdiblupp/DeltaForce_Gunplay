class DeltaForceTierEffect
{
	string Category;
	string Tier;
	float Recoil;
	float Sway;
	float ADS;
	float Precision;
	float HipFire;
	
	void DeltaForceTierEffect(string category = "Neutral", string tier = "Neutral", float recoil = 1.0, float sway = 1.0, float ads = 1.0, float precision = 1.0, float hipFire = 1.0)
	{
		Category = category;
		Tier = tier;
		Recoil = recoil;
		Sway = sway;
		ADS = ads;
		Precision = precision;
		HipFire = hipFire;
	}
	
	string GetLabel()
	{
		return Category + " " + Tier;
	}
}

class DeltaForceGunplayConfig
{
	static ref DeltaForceGunplayConfig m_Instance;
	
	float GlobalRecoilMultiplier;
	float GlobalSwayMultiplier;
	float GlobalAimSpeedMultiplier;
	float GlobalHipFireMultiplier;
	float GlobalPrecisionMultiplier;
	bool EnableHipFireNerf;
	bool EnableWeaponGeometryDamage;
	float GripRecoilReduction;
	float GripSwayReduction;
	float HeavyScopeAimSpeedPenalty;
	float LightScopeAimSpeedPenalty;
	float HipFireAccuracyDegradation;
	bool DebugMode;
	
	int HighCapMagazineThreshold;
	
	float Modifier_Buttstock_Recoil;
	float Modifier_Buttstock_Sway;
	float Modifier_PistolGrip_Recoil;
	float Modifier_PistolGrip_ADS;
	float Modifier_Foregrip_Recoil;
	float Modifier_Foregrip_ADS;
	float Modifier_Handguard_Sway;
	float Modifier_Handguard_Precision;
	float Modifier_Bipod_Sway;
	float Modifier_Bipod_ADS;
	float Modifier_Laser_HipFire;
	float Modifier_Flashlight_Sway;
	float Modifier_Flashlight_ADS;
	float Modifier_Bayonet_Sway;
	float Modifier_Bayonet_ADS;
	float Modifier_HighCapMag_ADS;
	float Modifier_HighCapMag_Sway;
	
	ref map<string, string> AttachmentTiers;
	ref map<string, ref DeltaForceTierEffect> AttachmentTierEffects;
	
	static DeltaForceGunplayConfig GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new DeltaForceGunplayConfig();
			m_Instance.Init();
		}
		return m_Instance;
	}
	
	void Init()
	{
		AttachmentTiers = new map<string, string>;
		AttachmentTierEffects = new map<string, ref DeltaForceTierEffect>;
		
		ApplyDefaultValues();
		Print("[DeltaForce Gunplay] Hardcoded gunplay config loaded");
	}
	
	void ApplyDefaultValues()
	{
		GlobalRecoilMultiplier = 1.08;
		GlobalSwayMultiplier = 1.08;
		GlobalAimSpeedMultiplier = 0.95;
		GlobalHipFireMultiplier = 1.0;
		GlobalPrecisionMultiplier = 1.0;
		EnableHipFireNerf = true;
		EnableWeaponGeometryDamage = true;
		GripRecoilReduction = 0.30;
		GripSwayReduction = 0.30;
		HeavyScopeAimSpeedPenalty = 0.3;
		LightScopeAimSpeedPenalty = 0.1;
		HipFireAccuracyDegradation = 0.65;
		DebugMode = false;
		
		HighCapMagazineThreshold = 30;
		
		Modifier_Buttstock_Recoil = 0.10;
		Modifier_Buttstock_Sway = 0.55;
		Modifier_PistolGrip_Recoil = 0.55;
		Modifier_PistolGrip_ADS = 1.10;
		Modifier_Foregrip_Recoil = 0.40;
		Modifier_Foregrip_ADS = 1.10;
		Modifier_Handguard_Sway = 0.70;
		Modifier_Handguard_Precision = 1.20;
		Modifier_Bipod_Sway = 0.40;
		Modifier_Bipod_ADS = 0.85;
		Modifier_Laser_HipFire = 0.50;
		Modifier_Flashlight_Sway = 1.03;
		Modifier_Flashlight_ADS = 0.96;
		Modifier_Bayonet_Sway = 1.06;
		Modifier_Bayonet_ADS = 0.94;
		Modifier_HighCapMag_ADS = 0.98;
		Modifier_HighCapMag_Sway = 1.03;
		
		InitDefaultTierEffects();
		InitDefaultAttachmentTiers();
		ApplyAdminConfig(DeltaForceGetAdminConfig());
	}
	
	void InitDefaultTierEffects()
	{
		AttachmentTierEffects.Clear();
		
		SetTierEffect("Foregrip_T1", "Foregrip", "T1", 0.70, 0.925, 1.03, 1.05, 0.90);
		SetTierEffect("Foregrip_T2", "Foregrip", "T2", 0.40, 0.85, 1.06, 1.10, 0.80);
		SetTierEffect("Foregrip_T3", "Foregrip", "T3", 0.10, 0.775, 1.08, 1.15, 0.70);
		
		SetTierEffect("PistolGrip_T1", "Pistol Grip", "T1", 0.70, 1.00, 1.05, 1.00, 0.90);
		SetTierEffect("PistolGrip_T2", "Pistol Grip", "T2", 0.40, 0.97, 1.10, 1.02, 0.80);
		SetTierEffect("PistolGrip_T3", "Pistol Grip", "T3", 0.10, 0.94, 1.15, 1.04, 0.70);
		
		SetTierEffect("Stock_T1", "Stock", "T1", 0.70, 0.85, 1.00, 1.05, 1.00);
		SetTierEffect("Stock_T2", "Stock", "T2", 0.40, 0.70, 0.99, 1.10, 1.00);
		SetTierEffect("Stock_T3", "Stock", "T3", 0.10, 0.55, 0.98, 1.15, 1.00);
		
		SetTierEffect("Handguard_T1", "Handguard", "T1", 1.00, 0.85, 1.02, 1.10, 0.90);
		SetTierEffect("Handguard_T2", "Handguard", "T2", 0.94, 0.70, 1.04, 1.20, 0.80);
		SetTierEffect("Handguard_T3", "Handguard", "T3", 0.88, 0.55, 1.06, 1.30, 0.70);
		
		SetTierEffect("Bipod_Special", "Bipod", "Special", 0.70, 0.43, 0.86, 1.16, 1.00);
		SetTierEffect("LightOptic_T1", "Optic", "T1", 1.00, 0.985, 1.05, 1.10, 1.00);
		SetTierEffect("HeavyOptic_T2", "Optic", "T2", 1.00, 0.97, 0.95, 1.20, 1.00);
		SetTierEffect("SniperOptic_T3", "Optic", "T3", 1.00, 0.955, 0.85, 1.30, 1.00);
		SetTierEffect("Laser_Tactical", "Laser", "Tactical", 1.00, 1.00, 1.00, 1.00, 0.55);
		SetTierEffect("Flashlight_Utility", "Flashlight", "Utility", 1.00, 1.04, 0.96, 1.00, 1.00);
		SetTierEffect("Bayonet_Heavy", "Bayonet", "Heavy", 1.00, 1.08, 0.92, 1.00, 1.00);
		SetTierEffect("Muzzle_T1", "Muzzle", "T1", 0.85, 0.925, 1.00, 1.05, 0.95);
		SetTierEffect("Muzzle_T2", "Muzzle", "T2", 0.70, 0.85, 0.99, 1.10, 0.90);
		SetTierEffect("Muzzle_T3", "Muzzle", "T3", 0.55, 0.775, 0.98, 1.15, 0.85);
		SetTierEffect("Suppressor_T1", "Suppressor", "T1", 0.85, 1.00, 0.98, 1.05, 0.95);
		SetTierEffect("Suppressor_T2", "Suppressor", "T2", 0.70, 1.01, 0.96, 1.10, 0.90);
		SetTierEffect("Suppressor_T3", "Suppressor", "T3", 0.55, 1.02, 0.94, 1.15, 0.85);
		SetTierEffect("WeaponWrap_Utility", "Wrap", "Utility", 1.00, 0.925, 0.97, 1.03, 1.00);
		SetTierEffect("StandardMag_Neutral", "Magazine", "Standard", 1.00, 1.00, 1.00, 1.00, 1.00);
		SetTierEffect("HighCap_Heavy", "Magazine", "HighCap", 1.02, 1.03, 0.98, 1.00, 1.00);
		SetTierEffect("Neutral", "Neutral", "Neutral", 1.00, 1.00, 1.00, 1.00, 1.00);
	}
	
	void InitDefaultAttachmentTiers()
	{
		AttachmentTiers.Clear();

		SetAttachmentTier("22LRSuppressor", "Suppressor_T1");
		SetAttachmentTier("ACOGOptic", "HeavyOptic_T2");
		SetAttachmentTier("ACOGOptic_6x", "HeavyOptic_T2");
		SetAttachmentTier("AK_Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("AK_FoldingBttstck", "Stock_T2");
		SetAttachmentTier("AK_PlasticBttstck", "Stock_T2");
		SetAttachmentTier("AK_PlasticHndgrd", "Handguard_T2");
		SetAttachmentTier("AK_RailHndgrd", "Handguard_T3");
		SetAttachmentTier("AK_Suppressor", "Suppressor_T2");
		SetAttachmentTier("AK_WoodBttstck", "Stock_T1");
		SetAttachmentTier("AK_WoodHndgrd", "Handguard_T1");
		SetAttachmentTier("AK74_Hndgrd", "Handguard_T1");
		SetAttachmentTier("AK74_WoodBttstck", "Stock_T1");
		SetAttachmentTier("AKS74U_Bttstck", "Stock_T2");
		SetAttachmentTier("ASH12Suppressor", "Suppressor_T2");
		SetAttachmentTier("Aug_Barrel", "Muzzle_T1");
		SetAttachmentTier("Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("BUISOptic", "LightOptic_T1");
		SetAttachmentTier("Expansion_ANPEQ15_Base", "Laser_Tactical");
		SetAttachmentTier("Expansion_ANPEQ15_Green", "Laser_Tactical");
		SetAttachmentTier("Expansion_ANPEQ15_GreenNV", "Laser_Tactical");
		SetAttachmentTier("Expansion_ANPEQ15_IR", "Laser_Tactical");
		SetAttachmentTier("Expansion_ANPEQ15_Red", "Laser_Tactical");
		SetAttachmentTier("Expansion_Kar98_Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("Expansion_M1A_RailAtt", "Handguard_T1");
		SetAttachmentTier("Expansion_MP5_RailAtt", "Handguard_T1");
		SetAttachmentTier("Expansion_PMII25Optic", "SniperOptic_T3");
		SetAttachmentTier("ExpansionDeltapointOptic", "LightOptic_T1");
		SetAttachmentTier("ExpansionEXPS3HoloOptic", "LightOptic_T1");
		SetAttachmentTier("ExpansionHAMROptic", "HeavyOptic_T2");
		SetAttachmentTier("ExpansionKar98ScopeOptic", "SniperOptic_T3");
		SetAttachmentTier("ExpansionReflexMRSOptic", "LightOptic_T1");
		SetAttachmentTier("Fal_FoldingBttstck", "Stock_T3");
		SetAttachmentTier("Fal_OeBttstck", "Stock_T2");
		SetAttachmentTier("FGRIP", "Foregrip_T1");
		SetAttachmentTier("FNP45_MRDSOptic", "LightOptic_T1");
		SetAttachmentTier("GhillieAtt_ColorBase", "WeaponWrap_Utility");
		SetAttachmentTier("Groza_Barrel_Grip", "Muzzle_T2");
		SetAttachmentTier("Groza_Barrel_Short", "Muzzle_T1");
		SetAttachmentTier("Groza_Barrel_Suppressor", "Suppressor_T2");
		SetAttachmentTier("Groza_LowerReceiver", "Handguard_T2");
		SetAttachmentTier("GrozaGL_LowerReceiver", "Handguard_T2");
		SetAttachmentTier("GrozaOptic", "HeavyOptic_T2");
		SetAttachmentTier("HandguardRIS", "Handguard_T2");
		SetAttachmentTier("HuntingOptic", "SniperOptic_T3");
		SetAttachmentTier("ImprovisedSuppressor", "Suppressor_T1");
		SetAttachmentTier("ItemSuppressor", "Suppressor_T1");
		SetAttachmentTier("KashtanOptic", "LightOptic_T1");
		SetAttachmentTier("KazuarOptic", "HeavyOptic_T2");
		SetAttachmentTier("KobraOptic", "LightOptic_T1");
		SetAttachmentTier("LongrangeOptic", "SniperOptic_T3");
		SetAttachmentTier("M4_CarryHandleOptic", "LightOptic_T1");
		SetAttachmentTier("M4_CQBBttstck", "Stock_T3");
		SetAttachmentTier("M4_MOLLEHandGuard", "Handguard_T2");
		SetAttachmentTier("M4_MPBttstck", "Stock_T2");
		SetAttachmentTier("M4_MPHndgrd", "Handguard_T2");
		SetAttachmentTier("M4_OEBHandGuard", "Handguard_T1");
		SetAttachmentTier("M4_OEBttstck", "Stock_T1");
		SetAttachmentTier("M4_PlasticHndgrd", "Handguard_T1");
		SetAttachmentTier("M4_RISHndgrd", "Handguard_T2");
		SetAttachmentTier("M4_Suppressor", "Suppressor_T2");
		SetAttachmentTier("M4_T3NRDSOptic", "LightOptic_T1");
		SetAttachmentTier("M68Optic", "LightOptic_T1");
		SetAttachmentTier("M9A1_Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("M9A1Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("Mag_Expansion_AWM_5rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_G36_30Rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_Kedr_20Rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_M14_10Rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_M14_20Rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_M9_15Rnd", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_MP7_40Rnd", "HighCap_Heavy");
		SetAttachmentTier("Mag_Expansion_MPX_50Rnd", "HighCap_Heavy");
		SetAttachmentTier("Mag_Expansion_Taser", "StandardMag_Neutral");
		SetAttachmentTier("Mag_Expansion_Vityaz_30Rnd", "StandardMag_Neutral");
		SetAttachmentTier("MakarovPBSuppressor", "Suppressor_T1");
		SetAttachmentTier("MK4Optic_Black", "HeavyOptic_T2");
		SetAttachmentTier("MK4Optic_ColorBase", "HeavyOptic_T2");
		SetAttachmentTier("MK4Optic_Green", "HeavyOptic_T2");
		SetAttachmentTier("MK4Optic_Tan", "HeavyOptic_T2");
		SetAttachmentTier("Mosin_Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("Mosin_Compensator", "Muzzle_T1");
		SetAttachmentTier("MP5_Compensator", "Muzzle_T1");
		SetAttachmentTier("MP5_PlasticHndgrd", "Handguard_T1");
		SetAttachmentTier("MP5_RailHndgrd", "Handguard_T2");
		SetAttachmentTier("MP5k_StockBttstck", "Stock_T2");
		SetAttachmentTier("MP7A2Suppressor", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_93RAuto", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_93RHeavy", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_93RLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_93RShort", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_93RTac", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_akmDmr", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Ash12Advanced", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Ash12long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Ash12Mid", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_ASVALlong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_AWMLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_ColtPythonCarbine", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_ColtPythonLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_ColtPythonShort", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_DeagleLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_DeagleTac", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_FPSSLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_G17cAdv", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_G17cCompetition", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_G18cLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M1014Long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M14_long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M14_short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M1911Tac", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M249Short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M250short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M700barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_M870ext", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Mini14long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_MP7Advanced", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_mp7long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_mp7long2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_mp7long3", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_P90Advanced", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_P90Long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_PKMShort", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_PKMShort2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QBZ191long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QBZ191short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QBZ95long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QBZ95short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QJB201Short_barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QJB201Tac_barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_QSZ92GTac", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_R93long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_R93Short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Revolver", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_Revolver_Short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SG553_barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SG553Advanced", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SG553long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SKS_short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SKS_Yugoslav", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SMG45_barrel", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SMG45Tac", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SR25Long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SR25Short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SV98long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SV98short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_SV98short2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_svdlong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_svdlong2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_UziLong", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_barrel_VSS_extended", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_AK74Bar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_AUGBar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_AWM", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_M249Bar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_M250Bar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_PKMBar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_QJB201Bar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_SR25", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_bipod_SV98Bar", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_AFG_Light", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Arisaka", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_AYA_ZFSG", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_BCM", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_CQRgen2", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_For_Shift", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Half_handstop", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Hera_CQR", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Kac", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Mag_AFG", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_MP7", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_P90_Std", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_RTK", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Rvg", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SCAR", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SF900A", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SHIFTbev", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SI", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SLR", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_SmallStopper", "Foregrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_STFG", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Tan_MK46K", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_Zenit_RK0", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_frog_ZenitRK125U", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_AYA_lance", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_AYA_lance_nobarrel", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_AYA_lance_S12K", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_CAA_RS47", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_CAA_RS47_PP19", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_CAA_RS47_S12K", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_competition", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_competition_nobarrel", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_competition_S12K", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_romania", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_slr", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_slr_extend", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_slr_PP19", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK_slr_S12K", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK100polymer", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK12_mlok", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AK12_mlok_extend", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AKS74U_handguard", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AKS74U_handguard_up", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_competition", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_KAC_RIS", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_KAC_URX", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_lvoa", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_mk18", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AR_std", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Ash12_Advanced", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AUG_competition", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AUG_Optic", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AUG_quadrail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AUG_Suppressed", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_AWMExtendedhandguard", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Bison_competition", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Citori725_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Citori725_Tac", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_FPSS_Advanced", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_G3_mlok", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_G3_mlok_extend", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_G3_Rail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_G3_shortRail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K416_A8", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K416_A8_extend", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K416_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K416_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K437_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K437_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_K437_Silencer", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_KC17_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_KC17_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_KC17_Silencer", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M1014_TriRaid", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M14_comp", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M14_ebr", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M14_rail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M249_Bipod", "Bipod_Special");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M249_TriRaid", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M249_TriRaidShort", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M7_dmr", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M7_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M7_short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M700_A3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M700_A6", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M700_handguard", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M700_nylon", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_M870_Rail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Marlin_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Marlin_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MCXLT_Long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MCXLT_Silencer", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Mini14_ebr", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Mini14_ergo", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Mini14_Std", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MK4_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MK4_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MK47_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MK47_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP5_PTR", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP5_PTR_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP5_striker", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP5K", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP5SD", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP7_Advanced", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_MP7_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_P90_Advanced", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_P90_FourTri", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_PKMZenitKit", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QBZ191_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QBZ95_mlok", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QCQ171_ForeGrip", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QCQ171_Tactical", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QCQ171_Tactical2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QJB201_Short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_QJB201_Tac", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_R93handguard", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SCARH_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SCARH_short", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SG553_Advanced", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SG553_long", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SKS_adv", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SKS_comp", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SMG45_Tactical", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SR25_Carbon", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SR25_cqb", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SR3M_Silencer", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SVD_rail", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_SVD_std", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Uzi_foregrip", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Uzi_military", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Vector_4PicHan", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Vector_ExtraExtented", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_Vector_Striker", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_handguard_ZenitKit", "Handguard_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_DBAL_X2", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_peq2", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_peq2_vanilla", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_PEQ25", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_Pistol", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_laser_prest4", "Laser_Tactical");
		SetAttachmentTier("My_DF_Weapons_Attachments_light", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_Baldr", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_BHD", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_BHDpistol", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_Odin", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_StrobeLight", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_Surf_X400", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_light_Warrior3S", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_762SL4", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_AK74", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_AKS", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_AMTBig", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_ARCompensator", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Ash12", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Ash12_2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Break", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_car15", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Citori725", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Common1", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Common1_2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Common1_3", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Common1_4", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_CSLR17", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_DTK2", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_FNAttenuator", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Hide_muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Hive", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Hive_HeatShield", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_HK_Noveske", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_HK437", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_HK437Silencer", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_HUXWRX", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_KC17_muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_LeverStd", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M14", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M249", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M250", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M250Silencer", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M40", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_M4A1", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MCX", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MCXLT_Sup", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MCXSilencer", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MCXSilencer2", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MK4_muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_MK47_muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_mp7", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_OPS", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Osprey", "Suppressor_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_P90", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_PAMuzzleBrake", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_PBS04", "Suppressor_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_QBZ191", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_QBZ191long", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_QCQ171", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_QCQ171Silencer", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_QJB201", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Russian9", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SG2", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SilHybrid46", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SKSHexagon", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_sr25cqbSilencer", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SR25Short", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SR3M", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SurF_SF3P", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_SV98", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_TwistedTempest", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_V6Muzzle", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_VKS", "Muzzle_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_Zen_DTK", "Muzzle_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_muzzle_ZeroRise", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_1p", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_acog_remake", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_ACOG93", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_aimpoint_pro", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_aimpoint_t2", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_aimpoint5000", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_Canted", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_compact_point", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_DeltaPoint", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_ekp_8", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_Fastfire", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_foldingRD", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_GlasslessRedDot", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_hamr", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_holo", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_holo2", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_indicatoradditive", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_LPM3", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_M157", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_okp_7", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_OSIGHT", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_pk_as", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_PSO7", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_Reddot", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_RMR", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_rmr2", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_sniperscope", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_sniperscope2", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_sniperscope3", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_sniperscope4", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_specter", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_Sro", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_ThermalAdditive", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_thermalreddot", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_thermalscope", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_thermalscope2", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_TrihawkPrismScope", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_UH1", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_Valday", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_VortexVenom", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_vudu", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_optic_ZenitBelomoPSO", "HeavyOptic_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_AKhandleCap", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Bison_mag_additional", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cantedMount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Common", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover1", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover4", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_cqbRailcover5", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover1", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover4", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_DDRailcover5", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_flashkill", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_FPSSMagwell", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_G3_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Hammer_M1911Tac", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_hydra", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover1", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover4", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_KACRailcover5", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_M14_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_M1911Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_M203", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_M700_nylon_cheekpad", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Marlin_HeavyTrigger", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Marlin_LightTrigger", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_MCXLT_SilencerCover", "Suppressor_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_MEO_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover1", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover4", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_metalRailcover5", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_PKM_Handle", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_PSGbass1", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_PSGbass2", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_PTMount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QBZ191highPiston", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QBZ191lowPiston", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QBZ95_Cheekpad", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QBZ95LongBowHandle", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QCQ171Heavy_Bolt", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QCQ171Light_Bolt", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QJB201highPiston", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_QJB201lowPiston", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_quickdraw", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_quickdraw_Black", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_quickdraw_Green", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_quickdraw_Tan", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_R93HeatShield", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_riser", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_S12K_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Shield", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_SKS_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_StickLoaderLifter", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_SVD_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Trigger_725Burst", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Trigger_725Twostage", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Trigger_M1911Tac", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Trigger_PSG", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover1", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover2", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover3", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover4", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_type2Railcover5", "Handguard_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_Universal_Cheekpad", "LightOptic_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_other_VSS_Mount", "Handguard_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_93RAdv", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_93RErgo", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_93RSteady", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AK100", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AK12", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AK74", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AKframe", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AKHouger", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AKpsg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_AKS", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ARProBalance", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ARProGrip", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ARProHeavy", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ARProStd", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ARpsg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ASVAL", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_CAR15_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_ColtPythonCarbine_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_DeagleErgo", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_FPSS_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_G18cAdv", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_G18cFriction", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_G18cSteady", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_g3", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_G3Balance", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_G3frame", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_g3psg1", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_GRALS", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_Izh_Polymer", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_K416_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_K437_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_KC17_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M1911Tac_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M249_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M250psg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M40A6", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M4A1_std", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_M7", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_Mag_MOE", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_Mag_MOE1", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_Mag_MOE2", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_MK4_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_MK47_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_MOE_K2", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_MORE", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_mp7psg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_mp7psg2", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_PKM", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QBZ191", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QBZ191heavy", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QBZ191light", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QBZ95_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QBZBalance", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QCQ171_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_QJB201_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_Revolver_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_RK3", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_scorpius", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_SG553_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_SI25", "PistolGrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_pisg_SMG45_pisg", "PistolGrip_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_ACR", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_ACR_BodyPad", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AdvanceART", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK_acradapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK19", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK74_Std", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AK74_Std_Pad", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AKS74U_Std", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AKSkeleton", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_ARtube", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Ash12Skeleton", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Ash12Sniper", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_AUG_StockPad", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_CAR15_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Citori725Short_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Citori725Tac_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_CoreCP", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_CQR2", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_CTR", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_E1", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_FPSS_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_FPSS_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Frame", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_HeraCQR", "Foregrip_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_HK416_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_K437_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_K437_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_KC17_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_KC17_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M1014Fixed", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M1014Retra", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M1014Retra_Short", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M14_ebr_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M14EBR_Stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M16", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M249_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M249_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M250_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M4A1_Std", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M7_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M7_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_M700_A6", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MagpulDT", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MarlinNonStock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MarlinOpenwork", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MarlinSport", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MarlinSteady", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MarlinTactical", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MBRS", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MCXLT_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MCXLT_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MDT", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MK47_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MK47_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MK47_Stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP5_HK_Endcap", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP5K", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP5K_2", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP5Tele", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP7_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_MP7_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_P90_StockPad", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PKM_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PKM_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PKMLight", "Flashlight_Utility");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PKMZenit", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PRS3", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PSG", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PT1", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_PT3", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_QBZ191_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_QCQ171_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_QCQ171_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_QJB201_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_QJB201_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Revolver_Stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_S12K_Std", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_S12KBumpStock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SCARH_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SG553_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SG553_acradapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SG553_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SI", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Sig1", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Sig2", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SL", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Slimline", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SMG45_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SMG45_acradapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SMG45_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Strike", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SVD_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_SVD_stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_UBR2", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Uzi_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Uzi_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Uzi_Stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Vector_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_Vector_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_VectorStriker", "Stock_T3");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_vltor", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_VSS_1913adapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_VSS_bufferadapter", "Stock_T2");
		SetAttachmentTier("My_DF_Weapons_Attachments_stock_VSS_Stock", "Stock_T1");
		SetAttachmentTier("My_DF_Weapons_DMR_M14_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_M14_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_M14_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_M14_50RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_DMR_Mini14_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_Mini14_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_Mini14_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_SR25_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_SR25_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_SVD_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_SVD_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_VSS_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_VSS_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_DMR_VSS_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_M249_100RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_M250_125RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_M250_75RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_PKM_125RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_PKM_75RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_QJB201_125RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_LMG_QJB201_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Magazine", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_93R_12RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_93R_18RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Pistols_93R_24RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Pistols_Deagle_13RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_Deagle_7RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_Glock18C_17RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_Glock18C_25RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Pistols_Glock18C_33RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Pistols_M1911_11RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Pistols_M1911_7RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_QSZ92G_15RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Pistols_Shockpistol_2RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AK12_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AK74_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AK74_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_30RndMag2", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_30RndMag3", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_40RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKM_70RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKS74U_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKS74U_45RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_AKS74U_75RndDrumMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_Ash12_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_Ash12_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AUG_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AUG_30RndPMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_AUG_60RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_G3_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_G3_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_G3_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_G3_50RndDrumMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_K437_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_K437_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_K437_60RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_KC17_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_KC17_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_KC17_75RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_30RndMag2", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_30RndPMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_60RndDrumMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_M4A1_60RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_M7_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M7_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_M7_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_MCXLT_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_PTR32_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_QBZ191_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_QBZ191_60RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_QBZ95_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_SCARH_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_SCARH_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_SCARH_50RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Rifles_SG553_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_SG553_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Rifles_SG553_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Shotguns_FPSS_12RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Shotguns_FPSS_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Shotguns_FPSS_6RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Shotguns_FPSS_WithMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Shotguns_Saiga12K_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Shotguns_Saiga12K_24RndDrumMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Shotguns_Saiga12K_5RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_Bison_32RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_Bison_64RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_MK4_24RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_MK4_48RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_MP5_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_MP5_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_MP5_50RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_MP7_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_MP7_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_MP7_40RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_MP7_60RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_P90_50RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_PP19_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_PP19_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_QCQ171_100RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_QCQ171_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_QCQ171_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_SMG45_25RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_SMG45_40RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_Uzi_25RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_Uzi_35RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_Uzi_45RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_Vector_20RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_Vector_30RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_SMG_Vector_40RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_SMG_Vector_70RndMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Snipers_AWM", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Snipers_AWM_5RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Snipers_M700", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Snipers_M700_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Snipers_M700_5RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Snipers_R93", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Snipers_R93_10RndMag", "StandardMag_Neutral");
		SetAttachmentTier("My_DF_Weapons_Snipers_R93_15RndExtMag", "HighCap_Heavy");
		SetAttachmentTier("My_DF_Weapons_Snipers_SV98", "SniperOptic_T3");
		SetAttachmentTier("My_DF_Weapons_Snipers_SV98_7RndMag", "StandardMag_Neutral");
		SetAttachmentTier("PistolLaserPointer", "Laser_Tactical");
		SetAttachmentTier("PistolOptic", "LightOptic_T1");
		SetAttachmentTier("PistolSuppressor", "Suppressor_T2");
		SetAttachmentTier("PP19_Bttstck", "Stock_T2");
		SetAttachmentTier("PSO11Optic", "HeavyOptic_T2");
		SetAttachmentTier("PSO1Optic", "HeavyOptic_T2");
		SetAttachmentTier("PSO6Optic", "HeavyOptic_T2");
		SetAttachmentTier("PUScopeOptic", "HeavyOptic_T2");
		SetAttachmentTier("Red9Bttstck", "Stock_T1");
		SetAttachmentTier("ReflexOptic", "LightOptic_T1");
		SetAttachmentTier("Saiga_Bttstck", "Stock_T2");
		SetAttachmentTier("SKS_Bayonet", "Bayonet_Heavy");
		SetAttachmentTier("SportingOptic", "LightOptic_T1");
		SetAttachmentTier("StarlightOptic", "HeavyOptic_T2");
		SetAttachmentTier("Suppressor9A91", "Suppressor_T2");
		SetAttachmentTier("TLRLight", "Flashlight_Utility");
		SetAttachmentTier("UniversalLight", "Flashlight_Utility");
		SetAttachmentTier("weaponBayonet", "Bayonet_Heavy");
		SetAttachmentTier("weaponFlashlight", "Flashlight_Utility");
		SetAttachmentTier("weaponFlashlightRail", "Flashlight_Utility");
		SetAttachmentTier("weaponGrip", "PistolGrip_T1");
		SetAttachmentTier("weaponGripAK", "PistolGrip_T2");
		SetAttachmentTier("weaponGripCQR", "Foregrip_T2");
		SetAttachmentTier("weaponGripKAAR", "Foregrip_T3");
		SetAttachmentTier("weaponLaser", "Laser_Tactical");
		SetAttachmentTier("weaponLaserPointer", "Laser_Tactical");
		SetAttachmentTier("weaponPistolGrip", "PistolGrip_T2");
	}
	
	void SetAttachmentTier(string className, string tierKey)
	{
		if (!AttachmentTiers) AttachmentTiers = new map<string, string>;
		AttachmentTiers.Set(className, tierKey);
	}
	
	void SetTierEffect(string key, string category, string tier, float recoil, float sway, float ads, float precision, float hipFire)
	{
		if (!AttachmentTierEffects) AttachmentTierEffects = new map<string, ref DeltaForceTierEffect>;
		AttachmentTierEffects.Set(key, new DeltaForceTierEffect(category, tier, recoil, sway, ads, precision, hipFire));
	}
	
	string GetAttachmentTier(string className, string fallbackTier = "Neutral")
	{
		if (AttachmentTiers && AttachmentTiers.Contains(className))
		{
			return AttachmentTiers.Get(className);
		}
		
		return fallbackTier;
	}
	
	DeltaForceTierEffect GetTierEffect(string tierKey)
	{
		if (AttachmentTierEffects && AttachmentTierEffects.Contains(tierKey))
		{
			return AttachmentTierEffects.Get(tierKey);
		}
		
		if (AttachmentTierEffects && AttachmentTierEffects.Contains("Neutral"))
		{
			return AttachmentTierEffects.Get("Neutral");
		}
		
		return new DeltaForceTierEffect();
	}
	
	void ResetToDefaults()
	{
		ApplyDefaultValues();
		Print("[DeltaForce Gunplay] Config reset to defaults plus admin JSON");
	}
	
	void ApplyAdminConfig(DeltaForceGunplayAdminConfig adminConfig)
	{
		if (!adminConfig)
		{
			return;
		}
		
		adminConfig.EnsureDefaults();
		GlobalRecoilMultiplier = adminConfig.GlobalRecoilMultiplier;
		GlobalSwayMultiplier = adminConfig.GlobalSwayMultiplier;
		GlobalAimSpeedMultiplier = adminConfig.GlobalAimSpeedMultiplier;
		GlobalHipFireMultiplier = adminConfig.GlobalHipFireMultiplier;
		GlobalPrecisionMultiplier = adminConfig.GlobalPrecisionMultiplier;
		EnableHipFireNerf = adminConfig.EnableHipFireNerf;
		EnableWeaponGeometryDamage = adminConfig.EnableWeaponGeometryDamage;
		DebugMode = adminConfig.DebugMode;
		HighCapMagazineThreshold = adminConfig.HighCapMagazineThreshold;
		
		array<ref DeltaForceAttachmentStatOverride> customStatOverrides = new array<ref DeltaForceAttachmentStatOverride>;
		foreach (DeltaForceAttachmentStatOverride statOverride : adminConfig.AttachmentStatOverrides)
		{
			if (statOverride && !IsDefaultAttachmentStatOverride(statOverride))
			{
				customStatOverrides.Insert(statOverride);
			}
		}
		
		foreach (DeltaForceAttachmentTierOverride tierAssignment : adminConfig.AttachmentTierOverrides)
		{
			if (tierAssignment && tierAssignment.ClassName != "" && tierAssignment.TierKey != "")
			{
				SetAttachmentTier(tierAssignment.ClassName, tierAssignment.TierKey);
			}
		}
		
		foreach (DeltaForceTierMultiplierOverride tierOverride : adminConfig.TierMultiplierOverrides)
		{
			ApplyTierMultiplierOverride(tierOverride);
		}
		
		foreach (DeltaForceAttachmentStatOverride customStatOverride : customStatOverrides)
		{
			ApplyAttachmentStatOverride(customStatOverride);
		}
	}
	
	bool IsDefaultAttachmentStatOverride(DeltaForceAttachmentStatOverride statOverride)
	{
		if (!statOverride || statOverride.ClassName == "")
		{
			return false;
		}
		
		if (!AttachmentTiers || !AttachmentTiers.Contains(statOverride.ClassName))
		{
			return false;
		}
		
		string tierKey = AttachmentTiers.Get(statOverride.ClassName);
		if (!AttachmentTierEffects || !AttachmentTierEffects.Contains(tierKey))
		{
			return false;
		}
		
		DeltaForceTierEffect effect = AttachmentTierEffects.Get(tierKey);
		if (!effect)
		{
			return false;
		}
		
		return statOverride.Category == effect.Category && statOverride.Tier == effect.Tier && FloatClose(statOverride.Recoil, effect.Recoil) && FloatClose(statOverride.Sway, effect.Sway) && FloatClose(statOverride.ADS, effect.ADS) && FloatClose(statOverride.Precision, effect.Precision) && FloatClose(statOverride.HipFire, effect.HipFire);
	}
	
	bool FloatClose(float a, float b)
	{
		float diff = a - b;
		if (diff < 0.0)
		{
			diff = -diff;
		}
		
		return diff < 0.0001;
	}
	
	void ApplyTierMultiplierOverride(DeltaForceTierMultiplierOverride tierOverride)
	{
		if (!tierOverride || tierOverride.TierKey == "")
		{
			return;
		}
		
		if (!AttachmentTierEffects || !AttachmentTierEffects.Contains(tierOverride.TierKey))
		{
			return;
		}
		
		DeltaForceTierEffect effect = AttachmentTierEffects.Get(tierOverride.TierKey);
		if (!effect)
		{
			return;
		}
		
		effect.Recoil *= tierOverride.Recoil;
		effect.Sway *= tierOverride.Sway;
		effect.ADS *= tierOverride.ADS;
		effect.Precision *= tierOverride.Precision;
		effect.HipFire *= tierOverride.HipFire;
	}
	
	void ApplyAttachmentStatOverride(DeltaForceAttachmentStatOverride statOverride)
	{
		if (!statOverride || statOverride.ClassName == "")
		{
			return;
		}
		
		string tierKey = "Admin_" + statOverride.ClassName;
		SetTierEffect(tierKey, statOverride.Category, statOverride.Tier, statOverride.Recoil, statOverride.Sway, statOverride.ADS, statOverride.Precision, statOverride.HipFire);
		SetAttachmentTier(statOverride.ClassName, tierKey);
	}
}
