<?php

$maxentries = 1000;
$csv_path = "msgs.csv";

if(!isset($_GET["cpu_name"]) || !isset($_GET["msg"]))
{
	header('Content-Type: application/json');
	echo "null";
	exit();
}

$cpu_name = $_GET["cpu_name"];
$msg = $_GET["msg"];

if(strlen($cpu_name)==0 || strlen($msg)==0)
{
	header('Content-Type: application/json');
	echo "false";
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
	if($mutex_handle==false)
	{
		header('Content-Type: application/json');
		echo "false";
		exit();
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

function eliminateOldEntries($data)
{
	global $maxentries;
	if(count($data)>($maxentries-1))
	{
		$newdata = array();
		$start = count($data)-($maxentries-1);
		for($i=$start; $i<count($data); $i++)
		{
			array_push($newdata, $data[$i]);
		}
		return $newdata;
	}
	return $data;
}

function addEntry($data, $cpu_name, $msg)
{
	array_push($data, array($cpu_name, $msg));
	return $data;
}

$data = readCSV($csv_path);
$data = eliminateOldEntries($data);
$data = addEntry($data, $cpu_name, $msg);
writeCSV($csv_path, $data);

mutex_unlock();

header('Content-Type: application/json');
echo "true";

?>