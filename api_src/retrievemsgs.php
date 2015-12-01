<?php

$csv_path = "msgs.csv";

if(!isset($_POST["cpu_name"]))
{
	exit();
}

$cpu_name = $_POST["cpu_name"];
if(strlen($cpu_name)==0)
{
	exit();
}

$mutex_handle = false;

function mutex_lock($giveup=14)
{
	global $mutex_handle;
	$mutex_handle = fopen("lock", "w+");
	$attempt = 0;
	while($mutex_handle==false && $attempt<$giveup)
	{
		sleep(.5);
		$mutex_handle = fopen("lock", "w+");
		$attempt++;
	}
}

function mutex_unlock()
{
	global $mutex_handle;
	if($mutex_handle!=false)
	{
		fclose($mutex_handle);
		$mutex_handle = false;
	}
}

mutex_lock();

function readCSV($csvPath)
{
	$file_handle = fopen($csvPath, 'r');
	if($file_handle==false)
	{
		return array();
	}
	$entries = array();
	while(!feof($file_handle))
	{
		array_push($entries, fgetcsv($file_handle));
	}
	fclose($file_handle);
	return $entries;
}

function writeCSV($csvPath, $data)
{
	$file_handle = fopen($csvPath, 'w+');
	for($i=0; $i<count($data); $i++)
	{
		fputcsv($file_handle, $data[$i]);
	}
	fclose($file_handle);
}

function array_remove($array, $index)
{
	array_splice($array, $index, 1);
	return $array;
}

function retrieveEntries($data, $cpu_name)
{
	$entries = array();
	for($i=0; $i<count($data); $i++)
	{
		$row = $data[$i];
		if($row[0]==$cpu_name)
		{
			array_push($entries, $row[1]);
			$data = array_remove($data, $i);
			$i--;
		}
	}
	return array("data"=>$data, "entries"=>$entries);
}

$data = readCSV($csv_path);
$info = retrieveEntries($data, $cpu_name);
$data = $info["data"];
$entries = $info["entries"];
writeCSV($csv_path, $data);

header('Content-Type: application/json');
echo json_encode($entries);

mutex_unlock();

?>