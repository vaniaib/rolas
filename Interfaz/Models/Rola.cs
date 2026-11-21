using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.IO;
using Avalonia.Media.Imaging;
using System.Collections.Generic;
using Avalonia.Platform;
namespace Interfaz.Models
{
    public partial class Rola: ObservableObject{
	public int IdRola { get; set; }
        public int IdPerformer { get; set; }
        public int IdAlbum { get; set; }
        public string Title { get; set; } = "";
        public string Performer { get; set; } = "";
        public string Album { get; set; } = "";
        public string Path { get; set; } = "";
        public string Genre { get; set; } = "";
        public int Track { get; set; }
        public int Year { get; set; }
	private string coverPath = "";
	private static readonly Dictionary<string, Bitmap> cacheImagenes = new();
	private static Bitmap ObtenerImagen(string ruta){
	    if (cacheImagenes.TryGetValue(ruta, out Bitmap? imagen))
		return imagen;
	    Bitmap nueva = ruta.StartsWith("avares://")
		? CargarRecurso(ruta)
		: new Bitmap(ruta);
	    cacheImagenes[ruta] = nueva;
	    return nueva;
	}
	private static Bitmap CargarRecurso(string ruta){
	    using var stream = AssetLoader.Open(new Uri(ruta));
	    return new Bitmap(stream);
	}
	public Bitmap Portada{
	    get{
		string directorio = System.IO.Path.Combine(
		    Environment.GetFolderPath(Environment.SpecialFolder.UserProfile),".local", "share", "rolas", "imagenes");
		if(!string.IsNullOrWhiteSpace(CoverPath) && File.Exists(CoverPath)){
		    return ObtenerImagen(CoverPath);
		}
		if (!string.IsNullOrWhiteSpace(Album)){
		    foreach (string ext in new[] { ".jpg", ".png", ".jpeg", ".webp" }){
			string ruta = System.IO.Path.Combine(directorio, Album + ext);
			if (File.Exists(ruta))
			    return ObtenerImagen(ruta);
		    }
		}
        return ObtenerImagen("avares://Interfaz/Assets/catlistening.png");
		// if (!string.IsNullOrWhiteSpace(CoverPath) && File.Exists(CoverPath))
		//     return new Bitmap(CoverPath);
		// Console.WriteLine($"Álbum: {Album}");
		// Console.WriteLine($"CoverPath: {CoverPath}");
		// Console.WriteLine($"Directorio: {directorio}");
		// if (!string.IsNullOrWhiteSpace(Album)){
		//     foreach (string ext in new[] { ".jpg", ".png", ".jpeg" }){
		// 	string ruta = System.IO.Path.Combine(directorio, Album + ext);
		// 	Console.WriteLine($"Buscando: {ruta}");
		// 	Console.WriteLine($"Existe: {File.Exists(ruta)}");
		// 	if(File.Exists(ruta)){
		// 	    Console.WriteLine($"PORTADA ENCONTRADA: {ruta}");
		// 	    //  return ruta;
		// 	    return new Bitmap(ruta);
			    
			}
		    }
		    //}
		
		//     return new Bitmap("avares://Interfaz/Assets/catlistening.png");
		//}
		//	}
	public string CoverPath{
	    get => coverPath;
	    set{
		//=>
		//SetProperty(ref coverPath, value);
		if (SetProperty(ref coverPath, value))
		    OnPropertyChanged(nameof(Portada));
	    }
	}
					       }
}

