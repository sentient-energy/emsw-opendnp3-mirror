import org.totalgrid.reef.protocol.dnp3.BinaryOutput;
import org.totalgrid.reef.protocol.dnp3.FilterLevel;
import org.totalgrid.reef.protocol.dnp3.TcpSettings;

class TestBinding {
	 static {
		 System.setProperty("org.totalgrid.reef.protocol.dnp3.nostaticload", "false");
		 System.loadLibrary("opendnp3");
	 }

	public static void main(String[] args){
		FilterLevel level = FilterLevel.LEV_DEBUG;
		System.out.println("Filter = " + level);
		TcpSettings s = new TcpSettings("127.0.0.1", 9999);
		System.out.println(s.getMAddress() + ":" + s.getMPort());
		System.out.println("Success!");
	}

}

