<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Foto da Equipe</title>
    <style>

        @font-face {
            font-family: 'AlarmClock'; /*a name to be used later*/
            src: url('{{asset('DS-DIGI.TTF')}}'); /*URL to font*/
        }

        body{
            background-color: black;
            @if ($team->exploded)
                color:red;
            @else
                color:lime;
            @endif
            font-size: 25em;
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
    @if ($team->exploded)
        <div id="name">explodiu</div>
    @else
        <div id="name">desarmada</div>
    @endif
    {{$team->time}}
</body>
</html>