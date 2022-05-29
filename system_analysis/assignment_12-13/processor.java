//procQ.java를 기반
package Assignment12_13;
import genDevs.modeling.*;

import java.util.Stack;

import GenCol.*;
import simView.*;

public class processor extends ViewableAtomic
{
	
	protected Queue q;
	protected Stack<job> stop_st;
	protected job job;
	protected job processing_job;
	protected double processing_time;
	
	protected int size;
	protected int proc_num;
	protected loss_msg loss_msg;
	protected double temp_time; // atomic model의 sigma(left ta)를 받아서 할당
	
	public processor()
	{
		this("processor", 20, 2);
	}

	public processor(String name, double Processing_time, int Size)
	{
		super(name);
		size = Size;
    
		addInport("in");
		addOutport("out1"); // transd solved
		addOutport("out2"); // transd loss    queue가 다 차면 loss port로 내보내겠다.
		
		proc_num = Integer.parseInt(name.substring(9));
		
		processing_time = Processing_time;
	}
	
	public void initialize()
	{
		q = new Queue();
		stop_st = new Stack<>();
		job = new job("", '0', -1);
		processing_job = new job("", '0', -1);
		temp_time = 0;
		loss_msg = new loss_msg("none", 0);
		
		holdIn("passive", INFINITY);
	}

	public void deltext(double e, message x)
	{
		Continue(e);
		if (phaseIs("passive"))
		{
			for (int i = 0; i < x.size(); i++)
			{
				if (messageOnPort(x, "in", i))
				{
					job = (job) x.getValOnPort("in", i);
					processing_job = job;
					holdIn("busy", job.burst_time);
				}
			}
		}
		else if (phaseIs("busy"))
		{
			for (int i = 0; i < x.size(); i++)
			{
				if (messageOnPort(x, "in", i))
				{
					job other_job = (job) x.getValOnPort("in", i);
					if (processing_job.priority > other_job.priority) { // 처리중인 job보다 우선순위가 높은 job이 들어오면 먼저 처리
						processing_job.burst_time = (int) sigma;
						stop_st.push(processing_job);
						
						processing_job = other_job;
						holdIn("busy", other_job.burst_time);
					}
					else if (processing_job.priority == other_job.priority) {
						stop_st.push(other_job);
					}
					else {
						if (q.size() < size) { // 설계한 q.size가 실제 할당된 q.size보다 작으면
							q.add(other_job);
						}
						else {
							temp_time = sigma;
							holdIn("loss", 0);
						}
					}
				}
			}
		}
	}
	
	public void deltint()
	{
		if (phaseIs("loss"))
		{ // 남아있는 ta를 temp_time에 할당. 다른 작업 처리하고 돌아와서도 진행할 수 있게
			holdIn("busy", temp_time);
		}
		else if(phaseIs("busy")) {
			if (!stop_st.isEmpty()) { // queue보다 stack을 먼저 확인, 우선순위가 동일하면 stack에 들어오므로
				processing_job = stop_st.peek();
				stop_st.pop();
				
				holdIn("busy", processing_job.burst_time);
			}
			else if (!q.isEmpty())
			{
				job = (job) q.removeFirst();
				
				holdIn("busy", job.burst_time);
			}
			else
			{
				job = new job("", '0', -1);
				loss_msg = new loss_msg("none", 0);
				holdIn("passive", INFINITY);
			}
		}
	}

	public message out()
	{
		message m = new message();
		
		if (phaseIs("busy"))
		{
			m.add(makeContent("out1", job));
		}
		else if(phaseIs("loss")) { // processor 몇번이 job을 놓쳤다.
			loss_msg = new loss_msg("processor" + proc_num + " : loss a job", proc_num);
			m.add(makeContent("out2", loss_msg));
		}
		return m;
	}	
	
	public String getTooltipText()
	{
		return
        super.getTooltipText()
        + "\n" + "queue length: " + q.size()
        + "\n" + "queue itself: " + q.toString()
        + "\n" + "stack length: " + stop_st.size()
        + "\n" + "stack itself: " + stop_st.toString()
		+ "\n" + "processing_job: " + processing_job.getName()
		+ "\n" + "priority: " + processing_job.priority
		+ "\n" + "burst_time: " + processing_job.burst_time;
	}

}



