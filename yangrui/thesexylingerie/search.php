<html>
<head><title>laozibushiweisuonan</title></head>

<body>
<?php
	$searchterm = trim($_POST['searchterm']);
	if(!$searchterm){
		echo '�ϴ����ܵ�����㶫���Ҳ����Ƽ���';
		exit;
	}
	
	if(!get_magic_quotes_gpc()){
		$searchterm = addslashes($searchterm);
	}
	
?>
</body>
</html>
