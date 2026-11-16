using System;
using System.Threading.Tasks;
namespace Interfaz.Models{

public class aplicacionMiner{
    public aplicacionMiner(){}
    public async Task mina(string ruta){
	Miner minero = new Miner(ruta);
	await minero.ejecuta();
    }
    // static void Main(){
    // 	string dir = Console.ReadLine();
    // 	Miner miner = new Miner(dir);
    // 	miner.ejecuta();
    // }
}
				  }
