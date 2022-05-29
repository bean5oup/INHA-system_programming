package Assignment12_13;
import GenCol.*;

public class job extends entity
{   
	Character priority;
	int burst_time;
	
	public job(String name, Character priority, int burst_time)
	{  
		super(name);  
		this.priority = priority;
		this.burst_time = burst_time;
	}
}
