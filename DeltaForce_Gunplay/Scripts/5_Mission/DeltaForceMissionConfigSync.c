modded class MissionServer extends MissionBase
{
	override void OnInit()
	{
		super.OnInit();
		DeltaForceGetAdminConfig();
		GetRPCManager().AddRPC("DeltaForceGunplayAdmin", "RPCDeltaForceGunplayAdminSettings", this, SingeplayerExecutionType.Both);
	}
	
	void RPCDeltaForceGunplayAdminSettings(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		PlayerIdentity requestedBy = PlayerIdentity.Cast(sender);
		if (!requestedBy)
		{
			return;
		}
		
		GetRPCManager().SendRPC("DeltaForceGunplayAdmin", "RPCDeltaForceGunplayAdminSettings", new Param1<DeltaForceGunplayAdminConfig>(DeltaForceGetAdminConfig()), true, requestedBy);
	}
}

modded class MissionGameplay extends MissionBase
{
	void MissionGameplay()
	{
		GetRPCManager().AddRPC("DeltaForceGunplayAdmin", "RPCDeltaForceGunplayAdminSettings", this, SingeplayerExecutionType.Both);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		GetRPCManager().SendRPC("DeltaForceGunplayAdmin", "RPCDeltaForceGunplayAdminSettings", new Param1<DeltaForceGunplayAdminConfig>(NULL), true, NULL);
	}
	
	void RPCDeltaForceGunplayAdminSettings(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<DeltaForceGunplayAdminConfig> data;
		if (!ctx.Read(data))
		{
			return;
		}
		
		DeltaForceSetAdminConfig(data.param1);
		Print("[DeltaForce Gunplay] Admin settings received from server");
	}
}
