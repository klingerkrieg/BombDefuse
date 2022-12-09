<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
    <style>

        @font-face {
            font-family: 'AlarmClock'; /*a name to be used later*/
            src: url('{{asset('DS-DIGI.TTF')}}'); /*URL to font*/
        }

        body{
            background-color: black;
            color:red;
            font-size: 35em;
            font-family: 'AlarmClock';
            align-items: center;
            justify-content: center;
            display: flex;
            height: 100%;
            flex-direction: column;
            overflow: hidden;
        }

        html {
            height: 100%;
        }

        #name{
            font-size:.2em;
        }

    </style>
</head>
<body>
    <div id="name">{{$team->team_name}}</div>
    {{$team->time}}
</body>
</html>