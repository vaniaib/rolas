using CommunityToolkit.Mvvm.ComponentModel;
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
	public string CoverPath{
        get => coverPath;
        set => SetProperty(ref coverPath, value);
	}
    }
}
