modded class UniversalTemperatureSourceLambdaBaseImpl : UniversalTemperatureSourceLambdaBase
{
	override void DryItemsInVicinity(UniversalTemperatureSourceSettings pSettings)
	{
		float distance;
		array<Object> nearestObjects = new array<Object>();

		vector pos = pSettings.m_Position;
		if (pSettings.m_Parent != null)
			pos = pSettings.m_Parent.GetPosition();

		GetGame().GetObjectsAtPosition(pos, pSettings.m_RangeMax, nearestObjects, null);
		foreach (Object nearestObject : nearestObjects)
		{
			ItemBase nearestItem = ItemBase.Cast(nearestObject);
			
			//! heat transfer to items (no in player possession)
			if (nearestItem && nearestItem.HasWetness() && nearestItem != pSettings.m_Parent && !nearestItem.IsInherited(Man))
			{
				distance = vector.Distance(nearestItem.GetPosition(), pSettings.m_Position);
				distance = Math.Max(distance, 0.1);	//min distance cannot be 0 (division by zero)
			
			#ifdef NAMALSK_SURVIVAL	
			//Remove additional wetness from namalsk
			if (nearestItem.GetWet() > 1)
			{
				nearestItem.AddWet(1 - nearestItem.GetWet());
			}
			#endif

				float dryModifier = 0;				
				
				if (nearestItem.GetWet() >= GameConstants.STATE_DAMP)
				{
					dryModifier = (-1 * pSettings.m_UpdateInterval * nearestItem.GetDryingIncrement("groundHeatSource")) / distance;
					Math.Clamp(dryModifier, nearestItem.GetWetMin(), nearestItem.GetWetMax());
					nearestItem.AddWet(dryModifier);
				}
				
				array<EntityAI> cargoEntities = new array<EntityAI>();
				nearestItem.GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, cargoEntities);
				foreach (EntityAI cargoEntity : cargoEntities)
				{
					ItemBase cargoItem = ItemBase.Cast(cargoEntity);
					if (cargoItem)
					{
						dryModifier = 0;
						if (cargoItem.GetWet() >= GameConstants.STATE_DAMP)
						{
							dryModifier = (-1 * pSettings.m_UpdateInterval * cargoItem.GetDryingIncrement("groundHeatSource")) / distance;
							Math.Clamp(dryModifier, cargoItem.GetWetMin(), cargoItem.GetWetMax());
							cargoItem.AddWet(dryModifier);
						}
					}
				}
			}
			//Heating of items when next to a fireplace
			// Only allowed when heat source emits more than 300 degrees
			if (nearestItem && nearestItem.IsTemperatureVisible() && nearestItem != pSettings.m_Parent && !nearestItem.IsInherited(Man) && pSettings.m_Parent.GetTemperature() >= 300)
			{
				distance = vector.Distance(nearestItem.GetPosition(), pSettings.m_Position);
				distance = Math.Max(distance, 0.1);	//min distance cannot be 0 (division by zero)
				
				float heatModifier = 0;		
				float itemTemperature = nearestItem.GetTemperature();
				
				if (itemTemperature < 50) //Cap maximum temperatue from indirect heating to avoid boiling water
				{
					heatModifier = ( pSettings.m_UpdateInterval * 1.5) / distance; //1.5 degs per update needs to overpower enviroment cooling
					Math.Max(heatModifier, 0); //Allways positive number
					nearestItem.AddTemperature(heatModifier);
				}
			}			
		}
	}
}