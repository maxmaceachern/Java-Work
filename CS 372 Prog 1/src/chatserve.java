import java.io.*;
import java.net.*;
public class chatserve
{
	public static void main(String[] args) throws IOException
	{
		int portNum = Integer.parseInt(args[0]);
		ServerSocket sSocket = new ServerSocket(portNum);  //System independent class for the server side socket connection. http://docs.oracle.com/javase/tutorial/networking/sockets/clientServer.html
		System.out.println("Server running!");
		
		while (true){
			
			Socket sock = sSocket.accept( );           //Waits for the client and when the connection is established, the accept method returns a sock object with the same port as the client.               
		    System.out.println("Client Connected!");
			//Reads user input from keyboard
			BufferedReader serverInput = new BufferedReader(new InputStreamReader(System.in));
		    
			//Actually send the input to the client
		    OutputStream output = sock.getOutputStream(); 
		    PrintWriter serverWriter = new PrintWriter(output, true); //The PrintWriter function actually writes the information to the client
		 
		    //Receiving from response from server 
		    InputStream input = sock.getInputStream();
		    BufferedReader receiveRead = new BufferedReader(new InputStreamReader(input));
		    
		    String receiveMessage, sendMessage;               
		    //Loop used for the server side chatting
		    while(true)
		    {
		      if((receiveMessage = receiveRead.readLine()) != null)  
		      {
		    	 /* if(receiveMessage.equals("\\quit")){
			    	  System.out.println("You got here");
			    	  break;
			      }*/
		    	  System.out.println(receiveMessage);         //Output message
		      }
		      boolean connected = sock.isConnected() && !sock.isClosed();  
		      if(connected == false){
		    	  serverWriter.flush();
		    	  break;
		      }
		      //String testMsg = receiveRead.readLine();
		      //System.out.println(testMsg);
		      
		      if(receiveMessage.equals("\\quit")){
		    	  serverWriter.flush();
		    	  break;
		      }
		      sendMessage = serverInput.readLine();
		      if(sendMessage.equals("\\quit")){					//If the server enters "\\quit" then exit
		    	  serverWriter.println("DudeBro> Goodbye!");             
			      serverWriter.flush();
			      break;
		      }
		      serverWriter.println("DudeBro> " + sendMessage);        //Send message to client     
		      serverWriter.flush();
		    }
		}
	}                    
}          