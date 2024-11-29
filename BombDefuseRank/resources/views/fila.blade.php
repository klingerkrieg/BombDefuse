@extends('layouts.app')

@section('title', 'Próximas Equipes - Defuse The Bomb')

@section('content')

<main>

    <div class="container my-4 px-3">
        <img class="logo" src="{{asset('img/logo.png')}}">
        <div class="d-flex flex-column align-items-center mb-4">
            <h2 class="text-center fw-bold">Próximas Equipes</h2>
            <div class="d-flex gap-2">
                <a class="btn btn-dark" href="{{ route('home') }}">
                    <i class="bi bi-arrow-left"></i>
                    Voltar
                </a>
                @if(Auth::check())
                    <a class="btn btn-primary" href="{{ route('fila.create') }}">
                        Adicionar
                    </a>
                @endif
            </div>
        </div>

        <div class="table-responsive">
            <table class="table table-hover">
                <thead>
                    <tr>
                        <th scope="col">#</th>
                        <th scope="col">Equipe</th>
                        <th scope="col">Status</th>
                        @if(Auth::check())
                            <th scope="col">Ações</th>
                        @endif
                    </tr>
                </thead>
                <tbody>
                    @foreach ($equipes as $k=>$equipe)
                        <tr>
                            <th scope="row">{{$k+1}}</th>
                            <td>{{ $equipe->nome }}</td>
                            <td>
                                <div class="status_fila">{{ $equipe->status === 0 ? 'Em espera' : '' }}</div>
                            </td>
                            
                            @if(Auth::check())
                                <td class="d-flex gap-2">
                                    <a class="btn btn-sm btn-primary" href="{{ route('fila.edit', $equipe->id) }}">
                                        <i class="bi bi-pencil-square"></i>
                                    </a>

                                    <a class="btn btn-sm btn-success" href="https://wa.me/55{{ preg_replace('/[^\d]/', '', $equipe->whatsapp) }}?text={{$mensagemCodificada}}" target="_BLANK">
                                        <i class="bi bi-whatsapp"></i>
                                    </a>
                                    
                                </td>
                            @endif
                        </tr>
                    @endforeach
                </tbody>
            </table>
        </div>     
        
    </div>
   
</main>

<script>

    // Atualizando Fila
    setInterval(() => {
        window.location.reload();
    }, 30000)
    
</script>

@endsection

