/**
 *
 * @author Toni Pano
 */

package coe318.lab6;
import java.util.ArrayList;

public class Circuit {
    private static Circuit instance=null;
    private ArrayList<Resistor> resistors = new ArrayList<Resistor>();
    
    public static Circuit getInstance(){
	if(instance==null){
	    instance=new Circuit();
	}
	return instance;
    }
    
    private Circuit(){}
    
    public void addResistor(Resistor r){
	resistors.add(r);
    }
    
    @Override
    public String toString(){
	String s="";
	
	for(int i=0;i<resistors.size();i++){
	    s+=resistors.get(i)+"\n";
	}
	return s;
    }
    
    public static void main(String[]args){
	Circuit cir = Circuit.getInstance();
	Node[] node = {new Node(),new Node(),new Node()};
	
	//resistors add themselves to cirucit object when they are constructed.
	Resistor[] r ={new Resistor(01,node[0],node[1]),
			new Resistor(12,node[1],node[2]),
			new Resistor(20,node[2],node[0])};
	
	System.out.println(cir);
    }
}
