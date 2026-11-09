{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	packages = [ pkgs.jdk pkgs.gtk3 pkgs.gsettings-desktop-schemas ];
	shellHook = ''
		export XDG_DATA_DIRS="${pkgs.gsettings-desktop-schemas}/share/gsettings-schemas/${pkgs.gsettings-desktop-schemas.name}:${pkgs.gtk3}/share/gsettings-schemas/${pkgs.gtk3.name}:$XDG_DATA_DIRS"
	'';
}
