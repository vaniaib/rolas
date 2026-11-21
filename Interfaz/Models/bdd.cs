using System;
using System.IO;
using Microsoft.Data.Sqlite;
using System.Collections.Generic;
using System.Threading.Tasks;
namespace Interfaz.Models;   
public class Bdd{
    private readonly string rutaBDD;
    public Bdd(){
	string directorio = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile),".local", "share", "rolas");	    
	rutaBDD = Path.Combine(directorio, "rolas.db");
	    
    }
    public async Task<List<Rola>> getRolas(string busqueda = ""){
	List<Rola> rolas = new();
	using var conexion = new SqliteConnection($"Data Source={rutaBDD};Foreign Keys=True");
	await conexion.OpenAsync();
	string sql = @"
	SELECT
	r.id_rola,
	r.id_performer,
	r.id_album,
	r.title,
	p.name AS performer,
	a.name AS album,
	r.path,
	r.track,
	r.year,
	r.genre
	FROM rolas r
	LEFT JOIN performers p
	ON r.id_performer = p.id_performer
	LEFT JOIN albums a
	ON r.id_album = a.id_album
        WHERE
        r.title LIKE @busqueda COLLATE NOCASE
        OR p.name LIKE @busqueda COLLATE NOCASE
        OR a.name LIKE @busqueda COLLATE NOCASE
        OR r.genre LIKE @busqueda COLLATE NOCASE
	ORDER BY r.title;
	";
	using var comando = new SqliteCommand(sql, conexion);
	comando.Parameters.AddWithValue("@busqueda",$"%{busqueda}%");
	using var lector = await comando.ExecuteReaderAsync();
	while(await lector.ReadAsync()){
	    rolas.Add(new Rola{
		    IdRola = lector.GetInt32(0),
		    IdPerformer = lector.IsDBNull(1) ? 0 : lector.GetInt32(1),
		    IdAlbum = lector.IsDBNull(2) ? 0 : lector.GetInt32(2),
		    Title = lector.IsDBNull(3) ? "" : lector.GetString(3),
		    Performer = lector.IsDBNull(4) ? "" : lector.GetString(4),
		    Album = lector.IsDBNull(5) ? "" : lector.GetString(5),
		    Path = lector.IsDBNull(6) ? "" : lector.GetString(6),
		    Track = lector.IsDBNull(7) ? 0 : lector.GetInt32(7),
		    Year = lector.IsDBNull(8) ? 0 : lector.GetInt32(8),
		    Genre = lector.IsDBNull(9) ? "" : lector.GetString(9)
		});
	}	
	return rolas;
    }
    public async Task<List<Elemento>> getElementos(string tabla){
        List<Elemento> elementos = new();
        using var conexion = new SqliteConnection($"Data Source={rutaBDD};Foreign Keys=True");
        await conexion.OpenAsync();
        string sql = tabla switch{
            "albums" => @"
                SELECT id_album, name, CAST(year AS TEXT)
                FROM albums
                ORDER BY name",
            "performers" => @"
                SELECT id_performer, name, CAST(id_type AS TEXT)
                FROM performers
                ORDER BY name",
            "persons" => @"
                SELECT id_person, stage_name, real_name
                FROM persons
                ORDER BY stage_name",
            "groups" => @"
                SELECT id_group, name, start_date
                FROM groups
                ORDER BY name",
            "types" => @"
                SELECT id_type, description, ''
                FROM types
                ORDER BY id_type",
            _ => throw new ArgumentException("Tabla no permitida")
        };
        using var comando = new SqliteCommand(sql, conexion);
        using var lector = await comando.ExecuteReaderAsync();
        while (await lector.ReadAsync()){
            elementos.Add(new Elemento{
		    Id = lector.GetInt32(0),
		    Nombre = lector.IsDBNull(1)
		    ? "" : lector.GetString(1),		
		    Detalle = lector.IsDBNull(2)
		    ? "" : lector.GetString(2),
		    Tabla = tabla
		});
        }
        return elementos;
    }
    
    public async Task editaRola(Rola rola){
	using var conexion = new SqliteConnection($"Data Source={rutaBDD};Foreign Keys=True");
	await conexion.OpenAsync();
	using var transaccion = conexion.BeginTransaction();
	try{
	    string sql = @"
	UPDATE rolas
	SET
	title = @title,
	id_performer = @performer,
	id_album = @album,
	track = @track,
	year = @year,
	genre = @genre
	WHERE id_rola = @id;
	";
	    using var comando = new SqliteCommand(sql, conexion, transaccion);
	    comando.Parameters.AddWithValue("@title", rola.Title);
	    comando.Parameters.AddWithValue("@performer", rola.IdPerformer);
	    comando.Parameters.AddWithValue("@album", rola.IdAlbum);
	    comando.Parameters.AddWithValue("@track", rola.Track);
	    comando.Parameters.AddWithValue("@year", rola.Year);
	    comando.Parameters.AddWithValue("@genre", rola.Genre);
	    comando.Parameters.AddWithValue("@id", rola.IdRola);
	    await comando.ExecuteNonQueryAsync();
	    if(rola.IdPerformer > 0){
		using var artista = new SqliteCommand(
		    @"UPDATE performers
                      SET name = @name
                      WHERE id_performer = @id",
		    conexion, transaccion);
                artista.Parameters.AddWithValue("@name", rola.Performer);
                artista.Parameters.AddWithValue("@id", rola.IdPerformer);
                await artista.ExecuteNonQueryAsync();
            }
            if(rola.IdAlbum > 0){
                using var album = new SqliteCommand(
                    @"UPDATE albums
                      SET name = @name
                      WHERE id_album = @id",
                    conexion, transaccion);		
                album.Parameters.AddWithValue("@name", rola.Album);
                album.Parameters.AddWithValue("@id", rola.IdAlbum);
                await album.ExecuteNonQueryAsync();
            }
            await transaccion.CommitAsync();
        }catch{
	    await transaccion.RollbackAsync();
	    throw;
	}
    }
		}

