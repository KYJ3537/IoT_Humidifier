<html>
<head>
    <!--한글 안깨지게 utf-8 형식으로-->
    <meta charset="utf-8" />

    <!--API 라이브러리 참조-->
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>

    <?php
        <!--에러가 발생하면 내용 표시-->
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
        <!--쿼리 연결 시 필요한 변수-->
        $mysql_host = "localhost";
        $mysql_user = "root";
        $mysql_password = "kyjtest";
        $mysql_db = "test_db";
        <!--연결-->
        $conn = mysqli_connect($mysql_host, $mysql_user, $mysql_password, $mysql_db);
        <!--연결 실패 시 에러 출력-->
        if (!$conn) {
            die("Database Connect Error: " . mysqli_connect_error());
        }
        
        <!--test_tb 라는 쿼리 테이블 SELECT 모두 가져오기
        $sql = "SELECT * FROM test_tb";
        <!--연결된 쿼리에 앞 $sql에 쓰인 대로 익스큐트하고 결과값 담기
        $result = mysqli_query($conn, $sql);
        <!--결과값 줄 갯수가 0개보다 크다면
        if (mysqli_num_rows($result) > 0) {
            <!--줄 마지막과 같아질때까지 반복-->
            while ($row = mysqli_fetch_assoc($result)) {
                <!--배열로 담기-->
                $data_array[] = $row;
            }
            <!--$chart에 JSON 형식으로 인코딩-->
            $chart = json_encode($data_array);
        }
	<!--header 변수 안에 있는 rows 문자열들을 합치고 jsonData에 담음-->
            var jsonData = [header].concat(rows);

            <!--담은 jsonData를 구글 api 중 데이터테이블 배열을 새로 만들고 그 형식을 data에 담음-->
            var data = new google.visualization.arrayToDataTable(jsonData);

            <!--옵션 부모 변수 안에 title, hAxis, series, vAxes 자식 변수가 들어있음-->
            <!--API 규칙에 맞게 해당 값들을 저장-->

            var options = {
                title: '프로젝트 온습도 측정 데이터',
                hAxis: {
                    title: '~번째 캡처 기록'
                },
                series: {
                    <!--0(온도)는 기본 축으로 할당-->
                    0: { targetAxisIndex: 0 },
                    <!--1(습도)는 0과 반대되는 축 할당-->
                    1: { targetAxisIndex: 1 }
                },
                vAxes: {
                    0: {
                        title: '온도',
                        <!--최소, 최대 틱 설정-->
                        viewWindow: { min: 0, max: 100 }
                    },
                    1: {
                        title: '습도',
                        viewWindow: { min: 0, max: 100 }
                    }
                }
            };
            <!--chart_div라는 아이디를 getElementById를 통해 위치 가져오고 그 값을 새로운 구글 라인차트 api을 chart에 담음-->
            var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
            <!--결국 구글 API로 데이터를 차트로 그림-->
            chart.draw(data, options);
        }
    </script>
</head>
<body>

<!-- HTML형식, chart_div라는 박스 스타일은 가로 1200 높이 800 마진은 가운데 정렬-->
<div id="chart_div" style="width: 1200px; height: 800px; margin: auto; text-align: center;"></div>
</body>
</html>