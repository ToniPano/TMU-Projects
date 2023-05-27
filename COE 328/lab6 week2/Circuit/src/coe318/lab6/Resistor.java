package coe318.lab6;

/**
 *
 * @author Toni Pano
 */
public class Resistor {
    private static int totalR;//total number of resistor objects 
    private int id;//unique id number of resistor
    private double r;//resistance of resistor
    private Node [] nodes= new Node[2];//nodes which resistor connect to
    
    public Resistor(double resistance, Node node1, Node node2){
	id=totalR;
	totalR++;
	
	if(resistance<0){
	    throw new IllegalArgumentException("Resistance ("+resistance+") can't be negative.");
	}
	else r=resistance;
	
	if(node1==null){
	    throw new IllegalArgumentException("node1 can't be null.");
	}
	else nodes[0]=node1;
	
	if(node2==null){
	    throw new IllegalArgumentException("node2 can't be null.");
	}
	else nodes[1]=node2;
	
	Circuit.getInstance().addResistor(this);
    }
    
    public Node[] getNodes(){
	return nodes;
    }
    
    @Override
    public String toString(){
	return "R"+id+" "+nodes[0]+" "+nodes[1]+" "+r;
    }
    
    public static void main(String [] args){
	Resistor r = new Resistor(30,new Node(),new Node());
	System.out.println(r);
    }
}