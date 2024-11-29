<!doctype html>
<html lang="pt-br">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="csrf-token" content="{{ csrf_token() }}">
    <title>@yield('title')</title>
    <link rel="dns-prefetch" href="//fonts.gstatic.com">
    <link href="https://fonts.googleapis.com/css?family=Nunito" rel="stylesheet">
    <link href="{{ asset('css/app.css') }}" rel="stylesheet">
    <link href="{{ asset('css/style.css') }}" rel="stylesheet">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.11.3/font/bootstrap-icons.min.css">
    <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" type="image/x-icon">
    <script src="{{ asset('js/app.js') }}" defer></script>
</head>
<body>
    <header class="d-flex flex-wrap justify-content-center p-3 bg-dark">
      <a href="{{ route('home') }}" class="d-flex align-items-center mb-3 mb-md-0 me-md-auto link-body-emphasis text-decoration-none text-white">
        <span class="fs-4">Defuse The Bomb</span>
      </a>
      <ul class="nav nav-pills d-flex gap-3">
        <li class="nav-item">
          <a class="btn border rounded text-decoration-none text-white" href="{{ route('dashboard') }}"><i class="bi bi-bar-chart"></i> Dashboard</a>
        </li>
        <li class="nav-item">
          <a class="btn border rounded text-decoration-none text-white" href="{{ route('rank') }}"><i class="bi bi-trophy"></i> Rank</a>
        </li>
        <li class="nav-item">
          <a class="btn border rounded text-decoration-none text-white" href="{{ route('fila') }}"><i class="bi bi-list-ol"></i> Fila</a>
        </li>
        <li class="nav-item">
          <form action="{{ route('logout') }}" method="POST">
            @csrf
            <button class="btn btn-light" type="submit"><i class="bi bi-box-arrow-left"></i> Sair</button>
          </form>
        </li>
      </ul>
    </header>
    @yield('content')
</body>
</html>
