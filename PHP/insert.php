<?php
<!--글 안깨지게-->
header("Content-Type: text/html;charset=UTF-8");
<!--쿼리 연결 시 필요한 변수-->
$host = 'localhost';
$user = 'root';
$pw = 'kyjtest';
$dbName = 'test_db';

<!--mysqli 함수(mysql 연결) 호출 새로운거 하나 $mysqli 만들고 담기-->
$mysqli = new mysqli($host, $user, $pw, $dbName);
 
	<!--연결 체크-->
    if($mysqli){
		<!--문자 출력-->
		echo "Connected";
		<!--api 형식으로 주소에 담아진 temp, humi 값 $temp, $humi에 담기-->
		$temp = $_GET['temp'];
		$humi = $_GET['humi'];
		<!--문자 출력-->
		echo "<br/>Temperature ($temp)";
		echo "<br/>Humidity ($humi)";
		<!--INSERT 쿼리문 -> test_tb 테이블에 temp랑 hum 컬럼에 담아진 값들 넣기-->
		$query = "INSERT INTO test_tb (Temp, Hum) VALUES ('$temp','$humi')";
		<!--익스큐트-->
		mysqli_query($mysqli,$query);
		<!--성공 메세지 출력-->
		echo "<br/>Record";
    } else {
		<!--실패 메세지 출력-->
		echo "Fail";
    }
<!--쿼리 연결 종료-->
mysqli_close($mysqli);
?>
{
            <!--크지 않다면 데이터가 없음-->
            echo "데이터가 존재하지 않습니다.";
        }
        
        <!--쿼리 연결 종료-->
        mysqli_close($conn);
    ?>
    
    <!--자바스크립트로-->
    <script type="text/javascript">
        <!--구글 비주얼 api corechart 패키지 로드-->
        google.charts.load('current', { packages: ['corechart', 'line'] });
        <!--api 로드 될때까지 실행할 콜백 설정-->
        google.charts.setOnLoadCallback(drawChart);
        
        <!--차트그리는 해당 함수로 설정-->
        function drawChart() {
            <!--헤더, 줄, 배열(php형식) 설정-->
            var chart_array = <?php echo $chart; ?>;
            var header = ['Num', 'Temp', 'Hum'];
            var row = "";
            var rows = new Array();
            <!--j쿼리에 chart_array 배열은 첫번째 매개변수 전달, 인덱스는 객체의 키 의미, 아이템은 인덱스가 가진 값-->
            <!--인덱스만큼 반복-->
            jQuery.each(chart_array, function(index, item) {
                <!--row에 item.Num, item.temp 숫자 변환, item.Hum 숫자변환해서 담음 -->
                row = [
                    item.Num,
                    Number(item.Temp),
                    Number(item.Hum)
                ];
                <!--배열 끝에 추가함-->
                rows.push(row);
            }); 