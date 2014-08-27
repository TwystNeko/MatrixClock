<?php
$file = "icosahedron.obj";
$fh = @fopen($file,"r");
$faces = array();
$vertices = array();
$normals = array();

if($fh) { 
	while (($buffer = fgets($fh, 4096))!== false) { 
		// parse line. 
		switch ($buffer[0]) {
			case 'v':
				$line = explode(" ", trim($buffer));
			// Vertex, vertex normal, or vertex texcoord
				if($line[0] == 'v') { 
					$vertices[] = array(sprintf("%.4f",$line[1]),sprintf("%.4f",$line[2]),sprintf("%.4f",$line[3]));
				}
				break;
			case 'f':
				// it's a face. 
				$line = explode(" ",trim($buffer));
				$face = array();
				
				for ($i=1; $i < count($line); $i++) { 
					# code..
					$fp = explode('/',$line[$i]);
					$face[] = $fp[0]-1;
				} 
				
				$faces[] = $face;
				break;
			default:
				# code...
				break;
		}
	}
}
$fname = basename($file,".obj");
$vcount = count($vertices);
echo "<pre>";
echo 'float '.$fname.'_v['.$vcount.'][3] = {'."\n";
foreach($vertices as $vertex) { 
	echo "{".implode(',',$vertex)."},\n";
}
echo "};\n\n";
$fcount = count($faces);
$fscount = count($faces[0]);
echo 'int '.$fname.'_f['.$fcount.']['.$fscount.'] = {'."\n";
foreach($faces as $face) { 
	echo "{".implode(',',$face)."},\n";
}
echo "};\n\n";
$name = ucfirst($fname);

echo <<<COUT

#define MESH_FACES $fcount
#define MESH_VERTICES $vcount

COUT;

echo 'Point3d '.$name.'[MESH_VERTICES]';
echo <<<COUT

Point3d Normals[MESH_FACES];
Th3dtran Transform;
DepthMap depthMap[MESH_FACES];

bool PARALLEL_PROJECTION  = true;
// function prototypes 

void loadMesh() {

  // load mesh into the Point3d array. 

COUT;
echo '  for(int i = 0; i&lt;MESH_VERTICES;i++) { '."\n";

echo '    '.$name.'[i].lx = '.$fname.'_v[i][0];'."\n";
    echo '    '.$name.'[i].ly = '.$fname.'_v[i][1];'."\n";
    echo '    '.$name.'[i].lz = '.$fname.'_v[i][2];'."\n";
echo <<<COUT
  }
}
COUT;

?>