modded class ItemBase
{
	override void InitItemVariables()
	{
		super.InitItemVariables();
		
		if (CanHaveTemperature())
		{
			Mission mission = GetGame().GetMission();
			if (mission && mission.GetWorldData())
			{
				SetTemperature( mission.GetWorldData().GetBaseEnvTemperature() );
			}
		}
	}
	
	bool CanHaveTemperature_SybInternalAccess()
	{
		return CanHaveTemperature();
	}
	
	override void ProcessItemTemperature( float delta, bool hasParent, bool hasRootAsPlayer, ItemBase refParentIB )
	{
		if (!CanHaveTemperature())
			return;
		
		if ( !hasRootAsPlayer )
		{

		#ifdef NAMALSK_SURVIVAL	
		//Remove additional wetness from namalsk
		if(HasWetness())
		{
			float itemWetness = GetWet();
			if (itemWetness > 1)
			{
				AddWet(1 - itemWetness);
			}
		}
		#endif

			Mission mission = GetGame().GetMission();
			if (!mission || !mission.GetWorldData())
				return;
			
			float envTemperature = mission.GetWorldData().GetBaseEnvTemperature();
			float itemTemperature = GetTemperature();
			if ( !hasParent )
			{
				// cooling on ground
				if ( !IsFireplace() )
				{
					if (itemTemperature > envTemperature) 
					{
						AddTemperature( Math.Max( delta * GameConstants.TEMPERATURE_RATE_COOLING_GROUND, envTemperature - itemTemperature ));
					}
					else if  (itemTemperature < envTemperature) 
					{
						AddTemperature( Math.Min( delta * GameConstants.TEMPERATURE_RATE_COOLING_GROUND * -1.0, envTemperature - itemTemperature ));
					}
				}
			}
			else if ( refParentIB )
			{
				// cooling of an item inside other
				if ( !IsFireplace() )
				{
					if (refParentIB.CanHaveTemperature_SybInternalAccess())
					{
						float parentTemperature = refParentIB.GetTemperature();
						if (itemTemperature > parentTemperature) 
						{
							AddTemperature( Math.Max( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE , parentTemperature - itemTemperature ));
						}
						else if (itemTemperature < parentTemperature) 
						{
							AddTemperature( Math.Min( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE * -1.0 , parentTemperature - itemTemperature ));
						}
					}
					else
					{
						if (itemTemperature > envTemperature) 
						{
							AddTemperature( Math.Max( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE , envTemperature - itemTemperature ));
						}
						else if (itemTemperature < envTemperature) 
						{
							AddTemperature( Math.Min( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE * -1.0 , envTemperature - itemTemperature ));
						}
					}
				}
			}
		}
	}
};