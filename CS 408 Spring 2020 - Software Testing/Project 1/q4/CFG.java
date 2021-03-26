import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.HashMap;
import java.util.Stack;
import org.objectweb.asm.commons.*;
import org.objectweb.asm.tree.*;

public class CFG {
    Set<Node> nodes = new HashSet<Node>();
    Map<Node, Set<Node>> edges = new HashMap<Node, Set<Node>>();

    static class Node {
	int position;
	MethodNode method;
	ClassNode clazz;

	Node(int p, MethodNode m, ClassNode c) {
	    position = p; method = m; clazz = c;
	}

	public boolean equals(Object o) {
	    if (!(o instanceof Node)) return false;
	    Node n = (Node)o;
	    return (position == n.position) &&
		method.equals(n.method) && clazz.equals(n.clazz);
	}

	public int hashCode() {
	    return position + method.hashCode() + clazz.hashCode();
	}

	public String toString() {
	    return clazz.name + "." +
		method.name + method.signature + ": " + position;
	}
    }

    public void addNode(int p, MethodNode m, ClassNode c) {
	//create new node object	
	Node newNode = new Node(p, m, c);
	
	//check if node already exists
	if(nodes.contains(newNode)){
	  return;
	}

	//if it does not exist then add it
	nodes.add(newNode);

	//check if node already has an edge
	if(edges.containsKey(newNode)){
	  return;
	}

	//if it does not exist then add it
	Set<Node> n = new HashSet<Node>();
	edges.put(newNode, n);
    }

    public void addEdge(int p1, MethodNode m1, ClassNode c1,
			int p2, MethodNode m2, ClassNode c2) {
	//add the nodes
	addNode(p1, m1, c1);
	addNode(p2, m2, c2);

	//create node objects
	Node newOne = new Node(p1, m1, c1);
	Node newTwo = new Node(p2, m2, c2);

	//add edge between nodes
	Set<Node> newEdge = edges.get(newOne);
	
	//connect edge from one to two
	if(!newEdge.contains(newTwo)){
	  edges.get(newOne).add(newTwo);
	}
    }
	
    public void deleteNode(int p, MethodNode m, ClassNode c) {
	//create node to be deleted
	Node delNode = new Node(p, m, c);

	//delete from node list
	if(nodes.contains(delNode)){
	  nodes.remove(delNode);
	}

	//delete from edge list
	edges.remove(delNode);

	//delete connecting edges
	for(Set<Node> edgePair : edges.values()){
	  if(edgePair.contains(delNode)){
	    edgePair.remove(delNode);
	  }
	}
    }
	
    public void deleteEdge(int p1, MethodNode m1, ClassNode c1,
						int p2, MethodNode m2, ClassNode c2) {
	//create nodes
	Node newOne = new Node(p1, m1, c1);
	Node newTwo = new Node(p2, m2, c2);

	//check that nodes are not deleted
	if(!nodes.contains(newOne) || !nodes.contains(newTwo)){
	  return;
	}

	//delete edge between nodes
	if(edges.containsKey(newOne)){
	  edges.get(newOne).remove(newTwo);
	}
    }
	

    public boolean isReachable(int p1, MethodNode m1, ClassNode c1,
			       int p2, MethodNode m2, ClassNode c2) {
	//create nodes
	Node newOne = new Node(p1, m1, c1);
	Node newTwo = new Node(p2, m2, c2);

	//check that nodes are not deleted
	if(!nodes.contains(newOne) || !nodes.contains(newTwo)){
	  return false;
	}

	//use BFS to search
	Stack<Node> q = new Stack<Node>();
	q.add(newOne);
	
	//search through queue	
	while(!q.isEmpty()){
	  Node cur = q.pop();
	  
	  //current node is reachable
	  if(newTwo.equals(cur)){
	    return true;
	  }

	  //move to next node
	  if(!edges.get(cur).isEmpty()){
	    for(Node n : edges.get(cur)){
	      q.add(n);
	    }
	  }
	}
	//The node is not reachable
	return false;
    }
}
