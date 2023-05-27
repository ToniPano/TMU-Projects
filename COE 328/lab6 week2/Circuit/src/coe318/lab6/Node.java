package coe318.lab6;

/**
 *
 * @author Toni Pano
 */
public class Node {
    private static int totalNodes=0;
    
    private int id;
    
    public Node(){
	id=totalNodes;
	totalNodes++;
    }
    
    @Override
    public String toString(){
	return ""+id;
    }
}
