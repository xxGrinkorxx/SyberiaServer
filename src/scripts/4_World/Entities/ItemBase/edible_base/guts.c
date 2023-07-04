modded class Guts
{
	override bool ContainsAgent( int agent_id )
	{
		if (agent_id == eAgents.SALMONELLA)
		{
			return true;
		}
		return super.ContainsAgent(agent_id);
	}
}