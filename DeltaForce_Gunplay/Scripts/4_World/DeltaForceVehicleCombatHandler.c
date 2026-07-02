class DeltaForceVehicleCombatHandler
{
	static bool IsPlayerInVehicle(PlayerBase player)
	{
		if (!player) return false;
		
		if (player.IsInTransport())
		{
			return true;
		}
		
		return false;
	}
	
	static bool IsPlayerInHelicopter(PlayerBase player)
	{
		if (!player) return false;
		
		if (!player.IsInTransport()) return false;
		
		return false;
	}
	
	static bool ShouldDisableHipFireNerf(PlayerBase player)
	{
		if (!player) return false;
		
		if (player.IsInTransport())
		{
			DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
			if (config.DebugMode)
			{
				Print("[DeltaForce Gunplay] Vehicle detected - Hip-Fire Nerf DISABLED");
			}
			return true;
		}
		
		return false;
	}
	
	static string GetVehicleType(PlayerBase player)
	{
		if (!player) return "";
		
		if (player.IsInTransport())
		{
			return "vehicle";
		}

		return "";
	}
}
